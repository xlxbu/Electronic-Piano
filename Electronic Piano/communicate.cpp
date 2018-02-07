#include "communicate.h"
#include <QTime>
#include <iostream>
#include <sstream>
#include <fstream>
#include <QPainter>
#include <QApplication>
#include <QMoveEvent>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QFileSystemModel>
#include <algorithm>
#include <map>
//////////////////////////
#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <cstring>
#include <string>

////////////////////////////
#define mp3 1
#define piano 0
#define play_icon 1
#define pause_icon 0

int mode = mp3;
int play_button = play_icon;

//////////////////////////////Rana's code//////////////////////////
using namespace std;
map<string,string> mysongs;
static double seconds=0;
static string devID=" CreativeD ";
static string aplayCommands = " -d=-9 -N & ";
vector<string>notes;
int length=0;
ifstream savedRecords;
pthread_t threads;
/*
display author and song name as apposed to MP3 file name
update connect function to not add a new device everytime
make play note function inline!
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int PlaySong(string addr){ //play selected song
	stringstream ss;
	ss << "madplay " << addr << " -R 44100 --tty-control --output=wave:- -v 2> fileinfo.txt | aplay -D "<< devID << aplayCommands <<  endl;
	system(ss.str().c_str());
	return 0;
}

void *worker(void *arg){
	cout << "entering worker function!!\n";
	string add;
	for(char i=0; i < length; i++){
		char id = *((char *) (arg )+ i);
		add+= id;
	}
  cout << add << endl;
	PlaySong(add);
//	sleep(10);
//	system("p");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BTsetup(){
   //sets up bluetooth...
   system("/etc/init.d/bluetooth stop"); //stop bluetooth
   cout << "Sleeping... " << endl;
   sleep(5); //sleep 0.5 seconds
   cout << "Done Sleeping... " << endl;
   system("/etc/init.d/bluetooth start"); //start bluetooth  
   system("passkey-agent --default 0000 &");
   printf("BTsetup done!\n");
}

void BTscan(vector<vector<string> > &dlist){
   /// Gets all devices and saves them to a vector
   system("hcitool scan > devlist.txt");
   string devAddr,space, devName, line;
   ifstream devList;
   devList.open("devlist.txt");
   vector<string> temp;
   while(devList.good()){
       temp.clear();
       getline(devList, line);
       if(line.length() < 17) continue;
       unsigned pos = line.find("\t", 1);
       devAddr= line.substr(1,pos-1);
       devName = line.substr(pos+1,line.length()-pos);
       temp.push_back(devAddr);
       temp.push_back(devName);
       dlist.push_back(temp);
       cout << devAddr << " " << devName << endl; 
   }
}

int BTconnect(string name, string addr){   //modify asound file to indclude a new device don't forget to error check
 //first check if it already exists before adding to asound.conf
   cout << "Connecting to device..";
   if(addr.length() < 17){
       cout << "Error: Dev Address Wrong" << endl;
       return 1;
   }
   string pcmName;
   for(unsigned int i=0; i < name.length(); i++){
       if(i >= 10) break;
       if((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z'))
             pcmName+= name[i];
   }
   devID = pcmName;
   fstream asound;
   asound.open("/etc/asound.conf", ios::out | ios::app);//etc/asound.conf
   if(asound.is_open()){
       asound << "\npcm." << pcmName << "{ \n\t type bluetooth \n\t device \"" << addr << "\"\n }\n";    
       asound.close();
   }
   else{
       cout << "Error: couldn't open asound file" << endl;
       return 2;
   }
   cout << "Successful connection" << endl;
   return 0;
}

int PlayNote(string note){ //play note that was clicked. save the note in a list. 7white 6black
	double new_seconds = (double)time(NULL);
	if(new_seconds - seconds > 5)
		notes.clear();
   seconds = new_seconds;
	notes.push_back(note);
	stringstream ss;
	ss << "aplay -D " << devID << " " << note << ".wav " << aplayCommands << endl;
	cout << ss.str() << endl;
	system(ss.str().c_str()); //play it
	return 0;
}

int PlayBackPiano(){ //play list of saved notes
	savedRecords.open("records.txt", ios::in);
	if(!savedRecords.is_open()){
		cout << "ERROR: opening records file" << endl;
		return 1;
	}
/*
	string line;
	while(savedRecords.good()){
		getline(savedRecords,line);
		stringstream ss;
		ss << "aplay -D " << devID << " " << line << ".wav";
		system(ss.str().c_str());
	}
	savedRecords.close();
*/
	return 0;
}
string SongInfo(){
	cout << "inside song info" <<endl;
	ifstream Info;
	Info.open("fileinfo.txt");
	string info((istreambuf_iterator<char>(Info)), istreambuf_iterator<char>());
	cout << info <<endl;
	Info.close();
	return info;
} 

int ListSongs(){	//get a playlist
	mysongs.clear();
	cout << "start search";
	system("find /media/mmcblk0 -name \"*.mp3\"> songlist.txt");
	ifstream myfile;
	myfile.open("songlist.txt");
	string song, line;
	while(myfile.good()){
 	  	getline(myfile,line);
   	if(line.length() < 5)
 	      break;
   	int i=line.length()-1;
   	while(line[i] != '/' && i-- >= 0){;}
   	song= line.substr(i+1,line.length()-i-5);
   	mysongs[song]=line;    
	}
	return 0;
}

int pause(){
//		system("p");
//      cin << "p";
		return 0;
}
int stop(){
		system("q");
		return 0;
}

string NextSong (string currentSong, bool prevnext){
/*
cout << "current song is:   " << currentSong << endl;
for(map<string,string>::iterator it= mysongs.begin(); it != mysongs.end(); ++it){

   if((*it).first == currentSong){
       if(!prevnext){
           if(it == mysongs.begin()){
             //it = mysongs.begin();
             cout << "at the beginning of the list"<<endl;
           }
           else{--it;}
       }else{
           if(it == mysongs.end()){
             //it = mysongs.end();
				//--it;
             cout << "at the end of the list" << endl;
           }
           else{++it;}
//           ++it;
       }
       return (*it).second;
   }
}
cout << "ERROR! Song Not Found!" << endl;
return "";*/
for(map<string,string>::iterator it= mysongs.begin(); it != mysongs.end(); ++it){
   if((*it).first == currentSong){
       if(!prevnext){
           if(it == mysongs.begin())
             cout << "at the beginning of the list"<<endl;
           else{
					--it;
				}
       }else{
				++it;
           if(it == mysongs.end()){
				  --it;
             cout << "at the end of the list" << endl;
           }
       }
       return (*it).second;
   }
}

}
//
//for(map<string,string>::iterator it=mysongs.begin(); it != mysongs.end(); ++it)
       //cout << (*it).first << "**" << (*it).second << endl;

//myfile.close();
//return 0;
//}44 5 6 4 6 5 4 5 6 3 4 5 44 56 4 3 4 4 5 6 7 6 5 4 3 1 2 3 4
// 1 c
/* 2 d 3 e 4 f 5 g 6 a 7 b
*/
/////////////////////////////////////////////////////////////////////

/////////////////////////////Liangxiao's QT code//////////////////////////////

Communicate::Communicate(QWidget *parent)
    : QWidget(parent)
{
/*define buttons, sliders, listwidgets, time and so on*/
  QPushButton *button_save = new QPushButton(this);
  button_save->setGeometry(60, 10, 30, 30);
  button_save->setIcon(QIcon(QPixmap("/media/mmcblk0/Save.png")));
  button_save->setIconSize(QSize(30, 30));
  button_save->setStyleSheet("QPushButton{border: none;outline: none;}");

  button_mode = new QPushButton(this);
  button_mode->setGeometry(410, 40, 50, 50);
  button_mode->setIcon(QIcon(QPixmap("/media/mmcblk0/music.png")));
  button_mode->setIconSize(QSize(50, 50));
  button_mode->setStyleSheet("QPushButton{border: none;outline: none;}");
  mode = 0;

  QPushButton *button_quit = new QPushButton(this);
  button_quit->setGeometry(10, 10, 30, 30);
  button_quit->setIcon(QIcon(QPixmap("/media/mmcblk0/shutdown.png")));
  button_quit->setIconSize(QSize(30, 30));
  button_quit->setStyleSheet("QPushButton{border: none;outline: none;}");

  button_play = new QPushButton(this);
  button_play->setGeometry(100, 45, 50, 50);
  button_play->setIcon(QIcon(QPixmap("/media/mmcblk0/Crystal_Project_Player_play.png")));
  button_play->setIconSize(QSize(50, 50));
  button_play->setStyleSheet("QPushButton{border: none;outline: none;}");
  button_play->setCheckable(true);

  button_pause = new QPushButton(this);
  button_pause->setGeometry(100, 45, 50, 50);
  button_pause->setIcon(QIcon(QPixmap("/media/mmcblk0/Crystal_Project_Player_pause.png")));
  button_pause->setIconSize(QSize(50, 50));
  button_pause->setStyleSheet("QPushButton{border: none;outline: none;}");
  button_pause->hide();

  QPushButton *forward = new QPushButton(this);
  forward->setGeometry(170, 45, 50, 50);
  forward->setIcon(QIcon(QPixmap("/media/mmcblk0/fast_forward.png")));
  forward->setIconSize(QSize(50, 50));
  forward->setStyleSheet("QPushButton{border: none;outline: none;}");

  QPushButton *backforward = new QPushButton(this);
  backforward->setGeometry(30, 45, 50, 50);
  backforward->setIcon(QIcon(QPixmap("/media/mmcblk0/slow_backforward.png")));
  backforward->setIconSize(QSize(50, 50));
  backforward->setStyleSheet("QPushButton{border: none;outline: none;}");

//  label = new QLabel("0", this);
//  label->setGeometry(420, 190, 60, 80);

  label2 = new QLabel("", this);
  label2->setGeometry(400, 10, 60, 20);

  label3 = new QLabel("0", this);
  label3->setGeometry(360, 10, 20, 30);

  slider = new QSlider(Qt::Horizontal , this);
  slider->setGeometry(250, 10, 100, 30);

  spinbox = new QSpinBox(this);
  spinbox->setGeometry(300, 50, 60, 30);
  spinbox->hide();

  bricks = new Brick(30, 50, "/media/mmcblk0/piano.png");

  QPushButton *button_1 = new QPushButton(this);
  button_1->setGeometry(0, 190, 60, 80);
  button_1->setStyleSheet("QPushButton {background-color:transparent;}");
  button_1->setFlat(1);

  QPushButton *button_2 = new QPushButton(this);
  button_2->setGeometry(60, 190, 60, 80);
  button_2->setStyleSheet("QPushButton {background-color:transparent;}");
  button_2->setFlat(1);

  QPushButton *button_3 = new QPushButton(this);
  button_3->setGeometry(120, 190, 60, 80);
  button_3->setStyleSheet("QPushButton {background-color:transparent;}");
  button_3->setFlat(1);

  QPushButton *button_4 = new QPushButton(this);
  button_4->setGeometry(180, 190, 60, 80);
  button_4->setStyleSheet("QPushButton {background-color:transparent;}");
  button_4->setFlat(1);

  QPushButton *button_5 = new QPushButton(this);
  button_5->setGeometry(240, 190, 60, 80);
  button_5->setStyleSheet("QPushButton {background-color:transparent;}");
  button_5->setFlat(1);

  QPushButton *button_6 = new QPushButton(this);
  button_6->setGeometry(300, 190, 60, 80);
  button_6->setStyleSheet("QPushButton {background-color:transparent;}");
  button_6->setFlat(1);

  QPushButton *button_7 = new QPushButton(this);
  button_7->setGeometry(360, 190, 60, 80);
  button_7->setStyleSheet("QPushButton {background-color:transparent;}");
  button_7->setFlat(1);

  QPushButton *button_black1 = new QPushButton(this);
  button_black1->setGeometry(45, 100, 30, 100);
  button_black1->setStyleSheet("QPushButton {background-color:transparent;}");
  button_black1->setFlat(1);

  QPushButton *button_black2 = new QPushButton(this);
  button_black2->setGeometry(105, 100, 30, 100);
  button_black2->setStyleSheet("QPushButton {background-color:transparent;}");
  button_black2->setFlat(1);

  QPushButton *button_black3 = new QPushButton(this);
  button_black3->setGeometry(225, 100, 30, 100);
  button_black3->setStyleSheet("QPushButton {background-color:transparent;}");
  button_black3->setFlat(1);

  QPushButton *button_black4 = new QPushButton(this);
  button_black4->setGeometry(285, 100, 30, 100);
  button_black4->setStyleSheet("QPushButton {background-color:transparent;}");
  button_black4->setFlat(1);

  QPushButton *button_black5 = new QPushButton(this);
  button_black5->setGeometry(345, 100, 30, 100);
  button_black5->setStyleSheet("QPushButton {background-color:transparent;}");
  button_black5->setFlat(1);

  
  Bluetooth = new QPushButton(this);
  Bluetooth->setGeometry(160, 10, 30, 30);
  Bluetooth->setIcon(QIcon(QPixmap("/media/mmcblk0/Bluetooth.png")));
  Bluetooth->setIconSize(QSize(30, 30));
  Bluetooth->setStyleSheet("QPushButton{border: none;outline: none;}");  


  OpenFile = new QPushButton(this);
  OpenFile->setGeometry(110, 10, 30, 30);
  OpenFile->setIcon(QIcon(QPixmap("/media/mmcblk0/open.png")));
  OpenFile->setIconSize(QSize(30, 30));
  OpenFile->setStyleSheet("QPushButton{border: none;outline: none;}");

  l_File = new QListWidget(this);
  l_File->setSortingEnabled(true);
  l_File->hide();

  l_BluetoothDevice = new QListWidget(this);
  l_BluetoothDevice->setSortingEnabled(true);
  QListWidgetItem *None = new QListWidgetItem("None");
  l_BluetoothDevice->addItem(None);
  l_BluetoothDevice->hide();

  Connect = new QPushButton(this);
  Connect->setGeometry(210, 10, 30, 30);
  Connect->setIcon(QIcon(QPixmap("/media/mmcblk0/connect_creating.png")));
  Connect->setIconSize(QSize(30, 30));
  Connect->setStyleSheet("QPushButton{border: none;outline: none;}");

  Info = new QTextEdit(this);
  Info->setGeometry(420, 100, 60, 180);
  Info->setText("Hello, world!\n");
//  Info->setPriority(256);
  Info->show();

  QTime qtime = QTime::currentTime();
  QString stime = qtime.toString();
  label2->setText(stime);
   
  startTimer(1000);

/*Build connections between buttons and slots*/
  connect(Connect, SIGNAL(clicked()), this, SLOT(Connect_dev()));
  connect(OpenFile, SIGNAL(clicked()), this, SLOT(SearchFile()));
  connect(Bluetooth, SIGNAL(clicked()), this, SLOT(SearchBluetooth()));
  connect(button_save, SIGNAL(clicked()), this, SLOT(OnSave()));
  connect(button_1, SIGNAL(clicked()), this, SLOT(Keyboard1()));
  connect(button_2, SIGNAL(clicked()), this, SLOT(Keyboard2()));
  connect(button_3, SIGNAL(clicked()), this, SLOT(Keyboard3()));
  connect(button_4, SIGNAL(clicked()), this, SLOT(Keyboard4()));
  connect(button_5, SIGNAL(clicked()), this, SLOT(Keyboard5()));
  connect(button_6, SIGNAL(clicked()), this, SLOT(Keyboard6()));
  connect(button_7, SIGNAL(clicked()), this, SLOT(Keyboard7()));
  connect(button_black1, SIGNAL(clicked()), this, SLOT(Keyboard_black1()));
  connect(button_black2, SIGNAL(clicked()), this, SLOT(Keyboard_black2()));
  connect(button_black3, SIGNAL(clicked()), this, SLOT(Keyboard_black3()));
  connect(button_black4, SIGNAL(clicked()), this, SLOT(Keyboard_black4()));
  connect(button_black5, SIGNAL(clicked()), this, SLOT(Keyboard_black5()));
  connect(button_mode, SIGNAL(clicked()), this, SLOT(OnMode()));
  connect(button_quit, SIGNAL(clicked()), qApp, SLOT(quit()));
  connect(slider, SIGNAL(valueChanged(int)), label3, SLOT(setNum(int)));
  connect(spinbox, SIGNAL(valueChanged(int)), this, SLOT(setTitle(int)));
  connect(button_play, SIGNAL(clicked()), this, SLOT(Play()));
  connect(button_pause, SIGNAL(clicked()), this, SLOT(Pause()));
  connect(forward, SIGNAL(clicked()), this, SLOT(Nextsong()));
  connect(backforward, SIGNAL(clicked()), this, SLOT(Previoussong()));
//  connect(slider, SIGNAL(valueChanged(int)), this, SLOT(Volume()));

}

void Communicate::OnSave()
{
//  int val = label->text().toInt();
//  val++;
//  label->setText(QString::number(val));
	ofstream outputFile;
	if(notes.size() >= 2) //if its been a while since last note was played
		outputFile.open("records.txt", ios::trunc | ios::out);//create new file
	else{
		cout << "Nothing to be saved!" << endl;
		return;	
	}
	if(!outputFile.is_open()){
		cout << "ERROR: opening save file" << endl;
		return;
	}
	else{
		for(unsigned int i=0; i < notes.size(); i++)
			outputFile << notes[i] << endl; //save note
		outputFile.close();
	}
}

void Communicate::OnMode()
{
//  int val = label->text().toInt();
//  val--;
//  label->setText(QString::number(val));
  if(mode == mp3){
    button_mode->setIcon(QIcon(QPixmap("/media/mmcblk0/music.png")));
    mode = piano;
    Info->move(420, 100);
    Info->resize(60,180);
    l_File->hide();
  }else if(mode == piano){
    button_mode->setIcon(QIcon(QPixmap("/media/mmcblk0/MP3.png")));

    Info->move(220, 100);
    Info->resize(260,170);
//    l_File->hide();
    mode = mp3;
  }
}

void Communicate::Keyboard1(){
  if(mode != piano){
    cout << "please turn to piano mode!\n" << endl;
    return;
  }
  PlayNote("/media/mmcblk0/c");
}

void Communicate::Keyboard2(){
  if(mode != piano){
    cout << "please turn to piano mode!\n" << endl;
    return;
  }
  PlayNote("/media/mmcblk0/d");
}

void Communicate::Keyboard3(){
  if(mode != piano){
    cout << "please turn to piano mode!\n" << endl;
    return;
  }
  PlayNote("/media/mmcblk0/e");
}

void Communicate::Keyboard4(){
  if(mode != piano){
    cout << "please turn to piano mode!\n" << endl;
    return;
  }
  PlayNote("/media/mmcblk0/f");
}

void Communicate::Keyboard5(){
  if(mode != piano){
    cout << "please turn to piano mode!\n" << endl;
    return;
  }
  PlayNote("/media/mmcblk0/g");
}

void Communicate::Keyboard6(){
  if(mode != piano){
    cout << "please turn to piano mode!\n" << endl;
    return;
  }
  PlayNote("/media/mmcblk0/a");
}

void Communicate::Keyboard7(){
  if(mode != piano){
    cout << "please turn to piano mode!\n" << endl;
    return;
  }
  PlayNote("/media/mmcblk0/b");
}

void Communicate::Keyboard_black1(){
  if(mode != piano){
    cout << "please turn to piano mode!\n" << endl;
    return;
  }
  PlayNote("/media/mmcblk0/cb");
}

void Communicate::Keyboard_black2(){
  if(mode != piano){
    cout << "please turn to piano mode!\n" << endl;
    return;
  }
  PlayNote("/media/mmcblk0/eb");
}

void Communicate::Keyboard_black3(){
  if(mode != piano){
    cout << "please turn to piano mode!\n" << endl;
    return;
  }
  PlayNote("/media/mmcblk0/gb");
}

void Communicate::Keyboard_black4(){
  if(mode != piano){
    cout << "please turn to piano mode!\n" << endl;
    return;
  }
  PlayNote("/media/mmcblk0/fb");
}

void Communicate::Keyboard_black5(){
  if(mode != piano){
    cout << "please turn to piano mode!\n" << endl;
    return;
  }
  PlayNote("/media/mmcblk0/bb");
}

void Communicate::timerEvent(QTimerEvent *e){
  Q_UNUSED(e);
  QTime qtime = QTime::currentTime();
  QString stime = qtime.toString();
  label2->setText(stime);

  if(play_button == pause_icon && mode == piano){
    string line;
    if(savedRecords.good()){
	 getline(savedRecords,line);
	 stringstream ss;
	 ss << "aplay -D " << devID << " " << line << ".wav "<< aplayCommands << endl;;
	 system(ss.str().c_str());
    }else{
      button_play->show();
      button_pause->hide();
      repaint();
      play_button = play_icon;
    }
  }else if(play_button == play_icon && mode == piano)
	  savedRecords.close();
//  ioctl(fd, MIXER_WRITE(SOUND_MIXER_VOLUME), 10);
//  stringstream ss1;
//  string test = label3->text().toStdString();
//  ss1 << "aplay -A " << test;
//  system(ss1.str().c_str());
/*  if(threads.size()!=0){
  pthread_cancel(threads[0]);
  cout << "killed !\n";
  }
*/
  repaint();
}

void Communicate::setTitle(int val) {
  setWindowTitle(QString::number(val));
}

void Communicate::paintEvent(QPaintEvent *e){
  Q_UNUSED(e);
  QPainter painter(this);
  painter.drawImage(bricks->getRect(),bricks->getImage());
  printf("this code is running!\n");
}

void Communicate::moveEvent(QMoveEvent *e){
  int x = e->pos().x();
  int y = e->pos().y();
  
  QString text = QString::number(x) + "," + QString::number(y);

  setWindowTitle(text);
}

void Communicate::SearchBluetooth()
{ 
//  BTsetup();
//  label->setText("Searching for the Bluetooth Devices");
//  printf("searching\n");
  Info->setText("Searching for the Bluetooth Devices...\n");
  repaint();
  //sleep(500);
  clearItems();
  vector<vector<string> >  dev_list;
//  vector<para_dev> &dev_list;
  BTscan(dev_list);
//  cout << "i am here!" << endl;
//   cout << "size: " << dev_list.size() << endl;
//   cout << "size: " << dev_list[1].size() << endl;
//  vector<vector<string> >::size_type dev;
//  vector<string>::size_type it;
  unsigned int dev;
//  unsigned int it;
  for(dev = 0; dev < dev_list.size(); dev++){
      //cout << dev << " " << it << " " << endl;
//    cout << dev_list[dev][0]  << " " << dev_list[dev][1] << endl;
    string dev_name = dev_list[dev][1];
    QString dev_name_item = QString::fromStdString(dev_name);
//    cout << "I am here 2!" << endl;
    QListWidgetItem *device = new QListWidgetItem(dev_name_item);
//	device.setText(dev_name);
    l_BluetoothDevice->addItem(device);
    cout << endl;
  }
  l_BluetoothDevice->show();
  l_BluetoothDevice->move(10,50);
  l_BluetoothDevice->resize(300,150);
  Info->setText("Finish Searching, Please choose one to connect\n"); 
}

void Communicate::SearchFile(){
  Info->setText("Searching for MP3 files...\n");
//  clearItems();
  repaint();
  clearFiles();
  ListSongs();
  for(map<string,string>::iterator it= mysongs.begin(); it != mysongs.end(); ++it)
		l_File->addItem((*it).first.c_str());//--> songname
  l_File->show();
  l_File->move(0, 100);
  l_File->resize(220,170);
  Info->setText("Finish Searching for MP3 files\n");
  Info->move(220, 100);
  Info->resize(260,170);
}

void Communicate::Play(){
    button_play->hide();
    button_pause->show();//l_File->hide();
	string sinfo = SongInfo();
	cout << sinfo << endl;
    Info->setText(SongInfo().c_str());
    repaint();
    play_button = pause_icon;
    if(mode == mp3){
      string name = "None";
      string address = "";
      QListWidgetItem *item = l_File->currentItem();
      for(map<string,string>::iterator it= mysongs.begin(); it != mysongs.end(); ++it){
        if(item->text().toStdString().compare((*it).first.c_str()) == 0){
          name = item->text().toStdString();
          address = (*it).second.c_str();
          cout << (*it).second.c_str() << endl;
          break;
        }
      }
      if(item->text().toStdString().compare("None") == 0)
        return;
      ///////////////////////////////////////////////////////////////////// start multiprocess!! with play song function!
  		cout << "starting multiprocess thing!" << endl;
  		length = address.length();
  		char *p;
  		//pthread_t *threads; //create thread
  		//threads = (pthread_t *) malloc(sizeof(pthread_t)); // Allocate memory
  		p = (char *) malloc((1+address.length())*sizeof(char));  // yes, memory leak, don't worry for now
  		strcpy(p, address.c_str());
  		pthread_create(&threads, NULL, worker, (void *)(p));
		
  		pthread_join(threads, NULL);
  		pthread_cancel(threads);
      cout << "killed !\n";
      //PlaySong(address);
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      
    }else{
      PlayBackPiano();
    }
}

void Communicate::Pause(){
    button_play->show();
    button_pause->hide();
    repaint();
    play_button = play_icon;
    if(mode == mp3){
      pause();
      pthread_cancel(threads);
      system("killall madplay");
      cout << "killed !\n";
//		pthread_kill(threads);
//      string note_test = "10";
//      PlaySong(note_test);
    }else{
//      PlayBackPiano();
    }
}

void Communicate::Connect_dev(){
  vector<vector<string> >  dev_list;
  string name = "None";
  string address = "";
  QListWidgetItem *item = l_BluetoothDevice->currentItem();
  BTscan(dev_list);
  vector<vector<string> >::size_type dev;
//  vector<string>::size_type it;
  for(dev = 0; dev < dev_list.size(); dev++){
    if(item->text().toStdString().compare(dev_list[dev][1]) == 0){
      name = item->text().toStdString();
      address = dev_list[dev][0];
      break;
    }
  }
  if(name.compare("None")!=0)  
    BTconnect(name, address);
  l_BluetoothDevice->hide();
  Info->setText("Successfully connect to Bluetooth Device!\n");
}

void Communicate::clearItems(){  
  if (l_BluetoothDevice != 0) {
    l_BluetoothDevice->clear();
  }
  l_BluetoothDevice->addItem("None");
}

void Communicate::clearFiles(){  
  if (l_File != 0) {
    l_File->clear();
  }
//  l_File->addItem("None");
}

void Communicate::Volume(){  
  stringstream ss1;
  string test = label3->text().toStdString();
  ss1 << "madplay -A" << test;
  system(ss1.str().c_str());
}

void Communicate::Nextsong(){  
  system("killall madplay");
  PlaySong(NextSong (l_File->currentItem()->text().toStdString(), true));
	string sinfo = SongInfo();
	cout << sinfo << endl;
  Info->setText(SongInfo().c_str());
	//set the blue thing to the next song
	//Info->choose;
  repaint();
}

void Communicate::Previoussong(){  
  system("killall madplay");
  PlaySong(NextSong (l_File->currentItem()->text().toStdString(), false));
	string sinfo = SongInfo();
	cout << sinfo << endl;
  Info->setText(SongInfo().c_str());
  repaint();
}

Brick::Brick(int x, int y, char* png) {
  image.load(png);
//  destroyed = FALSE;
  rect = image.rect();
  rect.translate(x, y);
  rect.setRect(0, 100, 420,170);
}

Brick::~Brick() {
   std::cout << ("Brick deleted\n");
}

QRect Brick::getRect(){
  return rect;
}

QImage & Brick::getImage(){
  return image;
}

