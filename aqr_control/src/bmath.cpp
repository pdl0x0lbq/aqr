#include "bmath.h"
#include "math.h"
#include "mybase.h"
// 计算两个向量的点积

double dotProduct(const Vector& a, const Vector& b) {
    return a.x * b.x + a.y * b.y;
}

// 计算向量的模（长度）
double norm(const Vector& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

// 计算两个向量之间的夹角（以弧度为单位）
double angleBetweenVectors(const Vector& v1, const Vector& eAa) {
    // 计算点积
    double dot = dotProduct(v1, eAa);
    // 计算模
    double norm_v1 = norm(v1);
    double norm_eAa = norm(eAa);

    // 防止除以零
    if (norm_v1 == 0 || norm_eAa == 0) {
        return 0; // 或者抛出异常，这取决于你希望如何处理零向量的情况
    }

    // 计算余弦值
    double cos_theta = dot / (norm_v1 * norm_eAa);

    // 根据余弦值的符号确定夹角是在[0, π]还是[π, 2π]
    double angle = std::acos(cos_theta);
    if (v1.x >= 0) {
        if (v1.y >= 0) {
            // 第一象限
            angle = angle;
        } else {
            // 第四象限
            angle =  - angle;
        }
    } else {
        if (v1.y >= 0) {
            // 第二象限
            angle = angle;
        } else {
            // 第三象限
            angle = - angle;
        }
    }


    return angle;
}

double adjustAngle(double angle) {
    // 如果角度大于 π/2，计算其补角
    if (angle > PI / 2) {
        angle = -(M_PI - angle);
    }
        // 如果角度小于 -π/2，计算其补角并取反
    else if (angle < -PI / 2) {
        angle = M_PI + angle;
    }
    // 如果角度已经在 -π/2 到 π/2 范围内，则不需要调整
    return angle;
}
