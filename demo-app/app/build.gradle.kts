plugins {
    alias(libs.plugins.android.application)
}

android {
    namespace = "com.example.demo" //改为你工程文件的名字
    compileSdk = 34

    defaultConfig {
        applicationId = "com.example.demo" //改为你工程文件的名字
        minSdk = 24
        targetSdk = 34
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"

        externalNativeBuild {
            cmake {
                arguments += "-DANDROID_STL=c++_shared"
            }
        }

        ndk {
            abiFilters += listOf("arm64-v8a", "armeabi-v7a")
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro")
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }
    splits {
        abi {
            isEnable = false
        }
    }
}

dependencies {
    implementation(libs.appcompat)
    implementation(libs.material)
    implementation(libs.activity)
    implementation(libs.constraintlayout)
    implementation(files("src/main/jniLibs/Msc.jar"))
    testImplementation(libs.junit)
    androidTestImplementation(libs.ext.junit)
    androidTestImplementation(libs.espresso.core)
    implementation("com.squareup.okhttp3:okhttp:4.9.3")
    implementation(files("src/main/jniLibs/geometry_msgs_messages.jar"))//添加你需要的ros2库
    implementation(files("src/main/jniLibs/action_msgs_messages.jar"))
    implementation(files("src/main/jniLibs/rcljava.jar"))
    implementation(files("src/main/jniLibs/rcljava_common.jar"))
    implementation(files("src/main/jniLibs/rcl_interfaces_messages.jar"))
    implementation(files("src/main/jniLibs/std_msgs_messages.jar"))
    implementation(files("src/main/jniLibs/slf4j-api-2.0.9.jar"))
    implementation(files("src/main/jniLibs/slf4j-jdk14-2.0.9.jar"))
    implementation(files("src/main/jniLibs/slf4j-jdk14-2.0.9.jar"))
}
