//Liangxiao's main function code
#include "communicate.h"
#include <QTextStream>
#include <QDate>
#include <QTime>
#include <fstream>

int main(int argc, char *argv[])
{
  QTextStream out(stdout);
  QApplication app(argc, argv);

  Communicate window;
  window.move(0, 0);
//  window.resize(400, 800);
  window.setWindowTitle("Communicate");
  window.show();
  window.showFullScreen();
  

  QDateTime cdt = QDateTime::currentDateTime();   
  out << "The current datetime is " << cdt.toString() << endl;
  out << "The current date is " << cdt.date().toString() << endl;
  out << "The current time is " << cdt.time().toString() << endl;
   
  return app.exec();

  
   
  
}

