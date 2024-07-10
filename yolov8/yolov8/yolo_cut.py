import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
from rclpy.qos import QoSProfile, QoSReliabilityPolicy
from vision_msgs.msg import Detection2DArray, Detection2D, ObjectHypothesisWithPose
import cv2
import numpy as np
import time
from cv_bridge import CvBridgeError
from hobot_dnn import pyeasy_dnn as dnn
import sys
import os
current_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.append(current_dir)
import utils
from utils import xywh2xyxy, nms, draw_detections, class_names

class YOLOv8:
    def __init__(self, path, conf_thres=0.7, iou_thres=0.5):
        self.conf_threshold = conf_thres
        self.iou_threshold = iou_thres
        self.initialize_model(path)
    def initialize_model(self, path):
        self.session = dnn.load(path)
        self.get_input_details()
        self.get_output_details()
    def detect_objects(self, image):
        input_tensor = self.prepare_input(image)
        outputs, inference_time = self.inference(input_tensor)  
        self.boxes, self.scores, self.class_ids = self.process_output(outputs)
        return self.boxes, self.scores, self.class_ids    
    def prepare_input(self, image):
        self.img_height, self.img_width = image.shape[:2]
        input_img = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        input_img = cv2.resize(input_img, (640, 640))
        height, width = input_img.shape[0], input_img.shape[1]
        area = height * width
        yuv420p = cv2.cvtColor(input_img, cv2.COLOR_BGR2YUV_I420).reshape((area * 3 // 2,))
        y = yuv420p[:area]
        uv_planar = yuv420p[area:].reshape((2, area // 4))
        uv_packed = uv_planar.transpose((1, 0)).reshape((area // 2,))
        nv12 = np.zeros_like(yuv420p)
        nv12[:height * width] = y
        nv12[height * width:] = uv_packed
        return nv12
    def inference(self, input_tensor):
        start = time.perf_counter()
        outputs = self.session[0].forward(input_tensor)
        inference_time = (time.perf_counter() - start) * 1000
        return outputs, inference_time
    def process_output(self, output):
        predictions = np.squeeze(output[0].buffer).T
        scores = np.max(predictions[:, 4:], axis=1)
        predictions = predictions[scores > self.conf_threshold, :]
        scores = scores[scores > self.conf_threshold]    
        if len(scores) == 0:
            return [], [], []
        class_ids = np.argmax(predictions[:, 4:], axis=1)
        boxes = self.extract_boxes(predictions)
        indices = nms(boxes, scores, self.iou_threshold)
        print("indices len:", len(indices))
        return boxes[indices], scores[indices], class_ids[indices]   
    def extract_boxes(self, predictions):
        boxes = predictions[:, :4]
        boxes = self.rescale_boxes(boxes)
        boxes = xywh2xyxy(boxes)
        return boxes
    def rescale_boxes(self, boxes):
        input_shape = np.array([self.input_width, self.input_height, self.input_width, self.input_height])
        boxes = np.divide(boxes, input_shape, dtype=np.float32)
        boxes *= np.array([self.img_width, self.img_height, self.img_width, self.img_height])
        return boxes
    def draw_detections(self, image, draw_scores=True, mask_alpha=0.4):

        return draw_detections(image, self.boxes, self.scores,
                               self.class_ids, mask_alpha)
    def get_input_details(self):
        self.input_height = 640
        self.input_width = 640
    def get_output_details(self):
        pass
class YOLODetector(Node):
    def __init__(self):
        super().__init__('yolo_detector')
        self.subscription = self.create_subscription(Image, '/camera/color/image_raw', self.image_callback, QoSProfile(
                reliability=QoSReliabilityPolicy.BEST_EFFORT,
                depth=1
            ))
        self.center_publisher = self.create_publisher(Detection2DArray, '/object_center', 10)
        self.bridge = CvBridge()
        self.yolov8_detector = YOLOv8("/root/model_output/horizon_x3.bin", conf_thres=0.5, iou_thres=0.5)
        self.subscription  
        self.get_logger().info('YOLODetector node initialized and subscribed to /arm_rgb topic')
    def image_callback(self, msg):
        try:
            cv_image = self.bridge.imgmsg_to_cv2(msg, "bgr8")
            boxes, scores, class_ids = self.yolov8_detector.detect_objects(cv_image)
            if len(boxes) == 0:
                self.get_logger().info('No objects detected.')
            else:
                self.get_logger().info('Objects detected.')
                print("Type of outputs:", type(boxes))  
                detection_array_msg = Detection2DArray()
                for box, score, class_id in zip(boxes, scores, class_ids):
                    detection_msg = Detection2D()
                    detection_msg.bbox.center.x = float(box[0] + box[2]) / 2
                    detection_msg.bbox.center.y = float(box[1] + box[3]) / 2
                    detection_msg.bbox.size_x = float(box[2] - box[0])
                    detection_msg.bbox.size_y = float(box[3] - box[1])
                    hypothesis = ObjectHypothesisWithPose()
                    hypothesis.id = str(class_id)
                    hypothesis.score = float(score)
                    detection_msg.results.append(hypothesis)
                    detection_array_msg.detections.append(detection_msg)
                    label = class_names[class_id]
                    self.get_logger().info(f'Object detected: ({detection_msg.bbox.center.x}, {detection_msg.bbox.center.y}), Label: {label}')
                self.center_publisher.publish(detection_array_msg)                    
        except CvBridgeError as e:
            self.get_logger().error(str(e))
    def draw_detections(self, image, boxes, scores, class_ids, draw_scores=True, mask_alpha=0.4):
        return draw_detections(image, boxes, scores, class_ids, draw_scores, mask_alpha)
def main(args=None):
    rclpy.init(args=args)
    yolo_detector = YOLODetector()
    rclpy.spin(yolo_detector)
    yolo_detector.destroy_node()
    rclpy.shutdown()
if __name__ == '__main__':
    main()
