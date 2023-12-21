#include "DraggableFramelessWindow.h"

DraggableFramelessWindow::DraggableFramelessWindow(QWindow *parent) :
    QQuickWindow(parent) {
    setFlags(flags() | Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}
bool DraggableFramelessWindow::resizable() const {
    return m_resizable;
}

void DraggableFramelessWindow::setResizable(bool arg) {
    if (m_resizable != arg) {
        m_resizable = arg;
    }
}

void DraggableFramelessWindow::mousePressEvent(QMouseEvent *event) {
    qDebug() << "mousePressEvent";
    // 获取鼠标全局坐标 QPoint
    m_startPos = event->globalPos();
    qDebug() << "m_startPos：x:" << m_startPos.x() << "y:" << m_startPos.y();
    // 获取鼠标对窗口相对坐标 QPoint（我感觉是窗口的位置）
    m_oldPos = position();
    qDebug() << "m_oldPos：x:" << m_oldPos.x() << "y:" << m_oldPos.y();
    // 获取当前窗口大小
    m_oldSize = size();

    // 告诉系统 事件未被处理，可以继续传递
    event->ignore();

    // 继续执行默认事件
    QQuickWindow::mousePressEvent(event);
}

void DraggableFramelessWindow::mouseReleaseEvent(QMouseEvent *event) {
    // 获取鼠标对窗口相对坐标 QPoint（我感觉是窗口的位置）
    m_oldPos = position();

    // 继续执行默认事件
    QQuickWindow::mousePressEvent(event);
}

void DraggableFramelessWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        qDebug() << "左键触发"
                 << "鼠标移动";
        if (m_resizable && m_currentArea != Client) {
            qDebug() << "可拖动";
            setWindowGeometry(event->globalPos());
        }
    } else {
        QPoint pos = event->pos();
        m_currentArea = getMouseArea(pos);
        if (m_currentArea != Client) {
            qDebug() << "非左键触发 可拖动";
            setResizable(true);
        } else {
            setResizable(false);
        }
        // if (m_resizable) {
        setCursorIcon();
        // }
    }

    QQuickWindow::mouseMoveEvent(event);
}

DraggableFramelessWindow::MouseArea DraggableFramelessWindow::getMouseArea(const QPoint &pos) {
    int x = pos.x();
    int y = pos.y();
    int w = width();
    int h = height();
    MouseArea area;
    if (x >= 0 && x <= pan && y >= 0 && y <= pan) {
        area = TopLeft;
    } else if (x > pan && x < (w - pan) && y >= 0 && y <= pan) {
        area = Top;
    } else if (x >= (w - pan) && x <= w && y >= 0 && y <= pan) {
        area = TopRight;
    } else if (x >= 0 && x <= pan && y > pan && y < (h - pan)) {
        area = Left;
    } else if (x >= (w - pan) && x <= w && y > pan && y < (h - pan)) {
        area = Right;
    } else if (x >= 0 && x <= pan && y >= (h - pan) && y <= h) {
        area = BottomLeft;
    } else if (x > pan && x < (w - pan) && y >= (h - pan) && y <= h) {
        area = Bottom;
    } else if (x >= (w - pan) && x <= w && y >= (h - pan) && y <= h) {
        area = BottomRight;
    } else {
        area = Client;
    }
    return area;
}

int DraggableFramelessWindow::readPan() const {
    return pan;
}

void DraggableFramelessWindow::setPan(int _pan) {
    pan = _pan;
}

void DraggableFramelessWindow::setWindowGeometry(const QPoint &pos) {
}

void DraggableFramelessWindow::setCursorIcon() {
    static bool unset = false;

    switch (m_currentArea) {
    case TopLeft:
    case BottomRight:
        unset = true;
        setCursor(Qt::SizeFDiagCursor);
        break;
    case Top:
    case Bottom:
        unset = true;
        setCursor(Qt::SizeVerCursor);
        break;
    case TopRight:
    case BottomLeft:
        unset = true;
        setCursor(Qt::SizeBDiagCursor);
        break;
    case Left:
    case Right:
        unset = true;
        setCursor(Qt::SizeHorCursor);
        break;
    default:
        if (unset) {
            unset = false;
            unsetCursor();
        }
        break;
    }
}
