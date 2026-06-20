#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv); // Qt 앱 초기화
    MainWindow   window;
    window.show();
    return app.exec(); // 이벤트 루프 시작
}
