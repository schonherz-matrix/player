#include "videosurface.h"

VideoSurface::VideoSurface(QObject *parent) : QAbstractVideoSurface(parent) {}

QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(
    QAbstractVideoBuffer::HandleType /*type*/) const {
  return QList<QVideoFrame::PixelFormat>{QVideoFrame::Format_RGB32};
}

bool VideoSurface::start(const QVideoSurfaceFormat &format) {
  return QAbstractVideoSurface::start(format);
}

void VideoSurface::stop() { QAbstractVideoSurface::stop(); }

bool VideoSurface::present(const QVideoFrame &frame) {
  if (frame.isValid()) {
    QVideoFrame cloneFrame(frame);
    cloneFrame.map(QAbstractVideoBuffer::ReadOnly);

    const QImage frame(
        cloneFrame.bits(), cloneFrame.width(), cloneFrame.height(),
        QVideoFrame::imageFormatFromPixelFormat(cloneFrame.pixelFormat()));

    m_transmitter.sendFrame(frame);
    m_videoWidget->setPixmap(QPixmap::fromImage(
        frame.scaled(m_videoWidget->size(), Qt::KeepAspectRatio)));

    cloneFrame.unmap();
  }

  return true;
}

void VideoSurface::setVideoWidget(QLabel *videoWidget) {
  m_videoWidget = videoWidget;
}
