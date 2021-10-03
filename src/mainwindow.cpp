#include "mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QTime>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
  ui->prevButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
  ui->stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
  ui->nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
  ui->muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

  m_mediaPlayer.setPlaylist(&m_mediaPlayList);
  m_mediaPlayer.setVideoOutput(&m_surface);
  m_surface.setVideoWidget(ui->videoWidget);

  connect(&m_mediaPlayer, &QMediaPlayer::positionChanged, this,
          &MainWindow::positionChanged);
  connect(&m_mediaPlayer, &QMediaPlayer::durationChanged, this,
          &MainWindow::durationChanged);
  connect(&m_mediaPlayer, &QMediaPlayer::stateChanged, this,
          &MainWindow::stateChanged);
  connect(&m_mediaPlayList, &QMediaPlaylist::currentIndexChanged, this,
          &MainWindow::currentIndexChanged);
  connect(ui->playList, &QListWidget::itemDoubleClicked, this,
          &MainWindow::itemDoubleClicked);
  connect(&m_mediaPlayList, &QMediaPlaylist::currentMediaChanged, this,
          &MainWindow::currentMediaChanged);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_soundSlider_valueChanged(int value) {
  m_mediaPlayer.setVolume(value);
}

void MainWindow::on_muteButton_toggled(bool checked) {
  m_mediaPlayer.setMuted(checked);

  if (checked) {
    ui->muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
    ui->soundSlider->setDisabled(true);
  } else {
    ui->muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    ui->soundSlider->setEnabled(true);
  }
}

void MainWindow::on_playButton_clicked() {
  if (m_mediaPlayer.state() == m_mediaPlayer.PlayingState)
    m_mediaPlayer.pause();
  else if (!m_mediaPlayer.media().isNull())
    m_mediaPlayer.play();
}

void MainWindow::on_autoPlay_stateChanged(int checked) {
  if (checked == Qt::Checked)
    m_mediaPlayList.setPlaybackMode(QMediaPlaylist::Sequential);
  else if (checked == Qt::Unchecked)
    m_mediaPlayList.setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
}

void MainWindow::positionChanged(qint64 position) {
  if (!ui->timeSlider->isSliderDown())
    ui->timeSlider->setValue(position / 1000);

  auto duration = m_mediaPlayer.duration();
  QString format = "mm:ss";
  if (duration > 3600000) format = "hh:mm:ss";

  qint64 remaining = duration - position;
  ui->currentTimeLabel->setText(
      QTime::fromMSecsSinceStartOfDay(position).toString(format));

  ui->remainingTimeLabel->setText(
      '-' + QTime::fromMSecsSinceStartOfDay(remaining).toString(format));
  ui->durationLabel->setText(
      QTime::fromMSecsSinceStartOfDay(duration).toString(format));
}

void MainWindow::durationChanged(qint64 duration) {
  ui->timeSlider->setMaximum(duration / 1000);

  QString format = "mm:ss";
  if (duration > 3600000) format = "hh:mm:ss";

  ui->durationLabel->setText(
      QTime::fromMSecsSinceStartOfDay(duration).toString(format));
}

void MainWindow::stateChanged(QMediaPlayer::State state) {
  if (state == QMediaPlayer::StoppedState || state == QMediaPlayer::PausedState)
    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

  if (state == QMediaPlayer::PlayingState)
    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

  if (state == QMediaPlayer::StoppedState) {
    ui->currentTimeLabel->setText("--:--");
    ui->remainingTimeLabel->setText("--:--");
    ui->durationLabel->setText("--:--");
  }
}

void MainWindow::on_prevButton_clicked() {
  if (m_mediaPlayList.playbackMode() == QMediaPlaylist::CurrentItemOnce)
    m_mediaPlayList.setCurrentIndex(m_currentItemIdx - 1);
  else
    m_mediaPlayList.previous();
}

void MainWindow::on_stopButton_clicked() {
  m_mediaPlayer.stop();
  m_mediaPlayList.setCurrentIndex(-1);
}

void MainWindow::on_nextButton_clicked() {
  if (m_mediaPlayList.playbackMode() == QMediaPlaylist::CurrentItemOnce)
    m_mediaPlayList.setCurrentIndex(m_currentItemIdx + 1);
  else
    m_mediaPlayList.next();
}

void MainWindow::on_timeSlider_sliderMoved(int position) {
  m_mediaPlayer.pause();
  m_mediaPlayer.setPosition(position * 1000);
}

void MainWindow::on_addButton_clicked() {
  auto files =
      QFileDialog::getOpenFileUrls(this, tr("Open Animations"), QUrl(),
                                   tr("Animations (*.mp4 *.webm)"), nullptr);

  for (const auto& file : files) {
    m_mediaPlayList.addMedia(file);
    ui->playList->addItem(file.fileName());
  }
}

void MainWindow::on_removeButton_clicked() {
  if (m_mediaPlayer.state() == m_mediaPlayer.PlayingState) m_mediaPlayer.stop();

  for (auto i : ui->playList->selectedItems()) {
    if (m_currentItem == i) m_currentItem = nullptr;

    m_mediaPlayList.removeMedia(ui->playList->row(i));
    delete i;
  }
}

void MainWindow::on_clearButton_clicked() {
  if (m_mediaPlayer.state() == m_mediaPlayer.PlayingState) m_mediaPlayer.stop();

  m_currentItem = nullptr;
  m_mediaPlayList.clear();
  ui->playList->clear();
}

void MainWindow::currentIndexChanged(int position) {
  auto font = QFont();

  if (m_currentItem) m_currentItem->setFont(font);

  if (position >= 0) {
    auto item = ui->playList->item(position);

    if (item) {
      font.setBold(true);
      item->setFont(font);

      m_currentItem = item;
      m_currentItemIdx = position;
    }
  }
}

void MainWindow::itemDoubleClicked(QListWidgetItem* item) {
  m_mediaPlayList.setCurrentIndex(ui->playList->row(item));
}

void MainWindow::currentMediaChanged(const QMediaContent& content) {
  setWindowFilePath(content.request().url().fileName());
}
