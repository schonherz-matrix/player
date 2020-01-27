#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidgetItem>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "videosurface.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_soundSlider_valueChanged(int value);

  void on_muteButton_toggled(bool checked);

  void on_nextButton_clicked();

  void on_stopButton_clicked();

  void on_prevButton_clicked();

  void on_timeSlider_sliderMoved(int position);

  void on_addButton_clicked();

  void on_removeButton_clicked();

  void on_clearButton_clicked();

  void on_playButton_clicked();

  void on_autoPlay_stateChanged(int checked);

  void positionChanged(qint64 position);

  void durationChanged(qint64 duration);

  void stateChanged(QMediaPlayer::State state);

  void currentIndexChanged(int position);

  void itemDoubleClicked(QListWidgetItem *item);

  void currentMediaChanged(const QMediaContent &content);

 private:
  Ui::MainWindow *ui;
  VideoSurface m_surface{this};
  QMediaPlaylist m_mediaPlayList{this};
  QMediaPlayer m_mediaPlayer{this};
  QListWidgetItem *m_currentItem{nullptr};
  int m_currentItemIdx{-1};
};

#endif  // MAINWINDOW_H
