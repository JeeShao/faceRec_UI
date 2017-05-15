#pragma once
#define CASCADE_FILE     ".\\haar\\haarcascade_frontalface_alt.xml"
#define CSV_FILE         ".\\train\\faces.csv"
#define TRAINMODEL_FILE  ".\\train\\facesModel.xml"
#define FACES_PIC_DIR    ".\\pic"
#define CV_CAP_PROP_FPS  60  //摄像头FPS值
#define FACE_NUM 30
#define RECOGNIZE_EIGEN  face::createEigenFaceRecognizer()
#define RECOGNIZE_FISHER face::createFisherFaceRecognizer()
#define RECOGNIZE_LBPH   face::createLBPHFaceRecognizer()
#define RECOGNIZE		 RECOGNIZE_LBPH
#define THRESHOLD        70
//人脸分辨率
#define FACE_WIDTH 92
#define FACE_HEIGHT 112
