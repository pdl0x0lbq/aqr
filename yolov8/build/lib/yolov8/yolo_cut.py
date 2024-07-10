import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2
import numpy as np
import time

from hobot_dnn import pyeasy_dnn as dnn
from utils import xywh2xyxy, nms, draw_detections

class YOLOv8:
    def __init__(self, path, conf_thres=0.7, iou_thres=0.5):
        self.conf_threshold = conf_thres
        self.iou_threshold = iou_thres

        # Initialize model
        self.initialize_model(path)

    def initialize_model(self, path):
        self.session = dnn.load(path)
        self.get_input_details()
        self.get_output_details()

    def detect_objects(self, image):
        input_tensor = self.prepare_input(image)
        outputs = self.inference(input_tensor)
        self.boxes, self.scores, self.class_ids = self.process_output(outputs)
        return self.boxes, self.scores, self.class_ids

    def prepare_input(self, image):
        # self.img_height, self.img_width = image.shape[:2]
        self.img_height, self.img_width = image.shape[:2]
        input_img = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        
        # Resize input image
        input_img = cv2.resize(input_img, (640,640))

        # Scale input pixel values to 0 to 1
        # input_img = input_img / 255.0
        
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
        inference_time = (time.perf_counter() - start) * 1000  # in milliseconds
        return outputs, inference_time

    def process_output(self, output):
        predictions = np.squeeze(output[0].buffer).T

        # Filter out object confidence scores below threshold
        scores = np.max(predictions[:, 4:], axis=1)
        predictions = predictions[scores > self.conf_threshold, :]
        scores = scores[scores > self.conf_threshold]
        
        
        if len(scores) == 0:
            return [], [], []

        # Get the class with the highest confidence
        class_ids = np.argmax(predictions[:, 4:], axis=1)

        # Get bounding boxes for each object
        boxes = self.extract_boxes(predictions)

        # Apply non-maxima suppression to suppress weak, overlapping bounding boxes
        indices = nms(boxes, scores, self.iou_threshold)
        print("indices len :",len(indices))
        return boxes[indices], scores[indices], class_ids[indices]

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
        self.subscription = self.create_subscription(Image, '/arm_rgb', self.image_callback, 10)
        self.publisher = self.create_publisher(Image, '/yolo_cut', 10)
        self.bridge = CvBridge()
        self.yolov8_detector = YOLOv8("/root/model_output/horizon_x3.bin", conf_thres=0.5, iou_thres=0.5)

    def image_callback(self, msg):
        try:
            cv_image = self.bridge.imgmsg_to_cv2(msg, "bgr8")
            # Detect objects
            boxes, scores, class_ids = self.yolov8_detector.detect_objects(cv_image)
            # Draw detections
            annotated_image = self.yolov8_detector.draw_detections(cv_image)
            # Publish annotated image
            self.publisher.publish(self.bridge.cv2_to_imgmsg(annotated_image, "bgr8"))
        except CvBridgeError as e:
            self.get_logger().error(str(e))

def main(args=None):
    rclpy.init(args=args)
    yolo_detector = YOLODetector()
    rclpy.spin(yolo_detector)
    yolo_detector.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
