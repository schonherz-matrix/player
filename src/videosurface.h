#ifndef VIDEOSURFACE_H
#define VIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QImage>
#include <QLabel>

#include "muebtransmitter.h"

class VideoSurface : public QAbstractVideoSurface {
  Q_OBJECT

 public:
  VideoSurface(QObject *parent = nullptr);

  // QAbstractVideoSurface interface
  QList<QVideoFrame::PixelFormat> supportedPixelFormats(
      QAbstractVideoBuffer::HandleType type) const override;
  bool start(const QVideoSurfaceFormat &format) override;
  void stop() override;
  bool present(const QVideoFrame &frame) override;
  void setVideoWidget(QLabel *videoWidget);

 private:
  MuebTransmitter &m_transmitter{MuebTransmitter::getInstance()};
  QLabel *m_videoWidget{nullptr};
};

#endif  // SURFACE_H
