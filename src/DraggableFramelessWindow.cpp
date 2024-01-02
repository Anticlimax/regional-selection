#include "DraggableFramelessWindow.h"

DraggableFramelessWindow::DraggableFramelessWindow(QWindow *parent) :
    QQuickWindow(parent) {
    setFlags(defaultFlags);
}
bool DraggableFramelessWindow::resizable() const {
    return m_resizable;
}

void DraggableFramelessWindow::setResizable(bool arg) {
    if (m_resizable != arg) {
        m_resizable = arg;
    }
    if (m_resizable) {
        removeFlags(Qt::WindowTransparentForInput);
    } else {
        addFlags(Qt::WindowTransparentForInput);
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
        // 这个过程会一直回调最新的鼠标位置
        if (m_resizable && m_currentArea != Client) {
            qDebug() << "可拖动"
                     << "mouseMovePos: x: " << event->globalPos().x() << "y: " << event->globalPos().y();
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
        setCursorIcon();
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

// &pos为当前鼠标拖动的位置
void DraggableFramelessWindow::setWindowGeometry(const QPoint &pos) {
    // 位移差值
    QPoint offset = m_startPos - pos;
    qDebug() << "offset: x " << offset.x() << "y " << offset.y();

    // 没动
    if (offset.x() == 0 && offset.y() == 0) {
        return;
    }

    static auto set_geometry_func = [this](const QSize &size, const QPoint &pos) {
        QPoint temp_pos = m_oldPos;
        QSize temp_size = minimumSize();
        // 如果传入的窗口宽度 大于 最小宽度，代表横向可拖动
        if (size.width() > minimumWidth()) {
            //  设置窗口X位置为传入的X位置
            temp_pos.setX(pos.x());
            // 设置窗口宽度为传入的窗口宽度
            temp_size.setWidth(size.width());
        } else {
            // 如果传入的窗口宽度 小于或等于 最小宽度，代表不可拖动
            // 如果当前鼠标位置是窗口左边
            if (m_currentArea == Left) {
                // 设置位置为  窗口原始位置 + 窗口原始高度 - 最小宽度
                // 没太懂
                temp_pos.setX(m_oldPos.x() + m_oldSize.width() - minimumWidth());
            }
        }

        // 如果传入的窗口高度 大于 最小高度，代表纵向可拖动
        if (size.height() > minimumHeight()) {
            // 设置窗口Y位置为传入的Y位置
            temp_pos.setY(pos.y());
            // 设置窗口高度为传入的窗口高度
            temp_size.setHeight(size.height());
        } else {
            // 如果传入的窗口高度 小于或等于 最小高度，代表不可拖动
            // 如果当前鼠标位置是窗口上边
            if (m_currentArea == Top) {
                // 设置位置为  窗口原始位置 + 窗口原始高度 - 最小高度
                temp_pos.setY(m_oldPos.y() + m_oldSize.height() - minimumHeight());
            }
        }

        setGeometry(QRect(temp_pos, temp_size));
        update();
    };

    switch (m_currentArea) {
    case TopLeft:
        // 如果是左上角，传参为 （（窗口原始大小+鼠标位移长度），（窗口原始位置 - 鼠标位移长度） ）
        set_geometry_func(m_oldSize + QSize(offset.x(), offset.y()), m_oldPos - offset);
        break;
    case Top:
        // QPoint(0, offset.y())为什么不直接写 offset.x(), offset.y() 因为鼠标可能会左右移动，但是窗口仅仅单项移动
        set_geometry_func(m_oldSize + QSize(0, offset.y()), m_oldPos - QPoint(0, offset.y()));
        break;
    case TopRight:
        // 向右移动 offset.x()为负值
        set_geometry_func(m_oldSize + QSize(-offset.x(), offset.y()), m_oldPos - QPoint(0, offset.y()));
        break;
    case Left:
        set_geometry_func(m_oldSize + QSize(offset.x(), 0), m_oldPos - QPoint(offset.x(), 0));
        break;
    case BottomLeft:
        set_geometry_func(m_oldSize + QSize(offset.x(), -offset.y()), m_oldPos - QPoint(offset.x(), 0));
        break;
    case Bottom:
        set_geometry_func(m_oldSize + QSize(0, -offset.y()), m_oldPos - QPoint(0, 0));
        break;
    case BottomRight:
        set_geometry_func(m_oldSize + QSize(-offset.x(), -offset.y()), m_oldPos - QPoint(0, 0));
        break;
    case Right:
        set_geometry_func(m_oldSize + QSize(-offset.x(), 0), m_oldPos - QPoint(0, 0));
        break;
    default:
        break;
    }
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

void DraggableFramelessWindow::addFlags(Qt::WindowFlags flags) {
    if (!m_resizable && !existFlags(flags)) {
        setFlags(this->flags() | flags);
        qDebug() << "addFlags:" << this->flags();
    }
}

void DraggableFramelessWindow::removeFlags(Qt::WindowFlags flags) {
    if (m_resizable && existFlags(flags)) {
        setFlags(this->flags() &= ~flags);
        qDebug() << "removeFlags:" << this->flags();
    }
}

bool DraggableFramelessWindow::existFlags(Qt::WindowFlags flags) {
    if (this->flags() & flags) {
        return true;
    } else {
        return false;
    }
}
