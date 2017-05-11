#pragma once
#define CASCADE_FILE     ".\\haar\\haarcascade_frontalface_alt.xml"
#define CSV_FILE         ".\\train\\faces.csv"
#define TRAINMODEL_FILE  ".\\train\\facesModel.xml"
#define FACES_PIC_DIR    ".\\pic"
#define RECOGNIZE_EIGEN  face::createEigenFaceRecognizer()
#define RECOGNIZE_FISHER face::createFisherFaceRecognizer()
#define RECOGNIZE_LBPH   face::createLBPHFaceRecognizer()
