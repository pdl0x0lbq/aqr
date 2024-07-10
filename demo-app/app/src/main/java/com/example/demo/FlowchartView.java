package com.example.demo;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.os.Handler;
import android.util.AttributeSet;
import android.view.View;

import java.util.List;

public class FlowchartView extends View {
    private Paint paint;
    private Paint paintText;
    private List<String> tasks;
    private List<int[]> coords;
    private int highlightedTaskIndex = -1;
    private boolean isHighlighted = false;
    private Handler handler = new Handler();
    private Runnable blinkRunnable;

    public FlowchartView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init() {
        // 初始化用于绘制矩形和线条的画笔
        paint = new Paint();
        paint.setColor(Color.parseColor("#1B263B")); // 深蓝色背景
        paint.setStrokeWidth(5);

        // 初始化用于绘制文本的画笔
        paintText = new Paint();
        paintText.setColor(Color.parseColor("#E0E0E0")); // 银色文本
        paintText.setTextSize(40);

        // 初始化闪烁任务
        blinkRunnable = new Runnable() {
            @Override
            public void run() {
                isHighlighted = !isHighlighted; // 切换高亮状态
                invalidate(); // 重新绘制视图
                handler.postDelayed(this, 500); // 每500毫秒切换一次
            }
        };
    }

    public void setTasksAndCoords(List<String> tasks, List<int[]> coords) {
        this.tasks = tasks;
        this.coords = coords;
        invalidate(); // 使用新数据重绘视图
    }

    public void startBlinking(int taskIndex) {
        this.stopBlinking();
        highlightedTaskIndex = taskIndex;
        handler.post(blinkRunnable); // 启动闪烁任务
    }

    public void stopBlinking() {
        handler.removeCallbacks(blinkRunnable); // 停止闪烁任务
        highlightedTaskIndex = -1;
        isHighlighted = false;
        invalidate(); // 重绘视图以移除高亮效果
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        if (tasks == null || coords == null || tasks.size() != coords.size()) {
            // 如果任务或坐标未设置或大小不匹配，不进行绘制
            return;
        }

        // 绘制任务
        for (int i = 0; i < tasks.size(); i++) {
            // 判断是否是高亮任务
            if (i == highlightedTaskIndex && isHighlighted) {
                paint.setColor(Color.parseColor("#4682B4")); // 浅蓝色高亮颜色
            } else {
                paint.setColor(Color.parseColor("#1B263B")); // 深蓝色普通颜色
            }

            // 测量文本的宽度
            float textWidth = paintText.measureText(tasks.get(i));
            float padding = 50; // 矩形左右的内边距
            float rectWidth = textWidth + padding * 2;

            // 绘制每个任务的矩形
            canvas.drawRect(coords.get(i)[0] - rectWidth / 2, coords.get(i)[1] - 50, coords.get(i)[0] + rectWidth / 2, coords.get(i)[1] + 50, paint);
            // 绘制每个任务的文本
            canvas.drawText(tasks.get(i), coords.get(i)[0] - textWidth / 2, coords.get(i)[1] + 10, paintText);

            // 绘制箭头
            if (i < tasks.size() - 1) {
                paint.setColor(Color.parseColor("#E0E0E0")); // 银色箭头
                canvas.drawLine(coords.get(i)[0], coords.get(i)[1] + 50, coords.get(i + 1)[0], coords.get(i + 1)[1] - 50, paint);
            }
        }
    }
}