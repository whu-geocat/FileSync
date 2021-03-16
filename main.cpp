#include "MyMainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyMainWindow w;
    w.show();

//    char cBuf[15];
//    std::ifstream inFile("C:\\Users\\geocat\\Desktop\\test\\in.txt", std::ios::in | std::ios::binary);
//    inFile.seekg(0, std::ios::end);
//    int iFileSize = inFile.tellg();
//    inFile.seekg(0, std::ios::beg);

//    inFile.read(cBuf, 15);
//    std::cout << "======Read content is: -----" << cBuf << "-----.\n";
//    inFile.close();

//    std::ofstream outFile("C:\\Users\\geocat\\Desktop\\test\\out.txt", std::ios::out | std::ios::binary);
//    outFile.write(cBuf, iFileSize);
//    outFile.close();

    return a.exec();
}
