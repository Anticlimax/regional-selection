#ifndef DRAGGABLEFRAMELESSWINDOW_H
#define DRAGGABLEFRAMELESSWINDOW_H

#include <QQuickWindow>

class DraggableFramelessWindow : public QQuickWindow {
    Q_OBJECT

    Q_PROPERTY(bool resizable READ resizable WRITE setResizable)
    Q_PROPERTY(int dragArea READ readPan WRITE setPan)

    enum MouseArea {
        TopLeft = 1,
        Top,
        TopRight,
        Left,
        Right,
        BottomLeft,
        Bottom,
        BottomRight,
        Client
    };

public:
    explicit DraggableFramelessWindow(QWindow *parent = nullptr);
    int readPan() const;
    void setPan(int _pan);
    bool resizable() const;
    void setResizable(bool arg);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    MouseArea getMouseArea(const QPoint &pos);

    void setWindowGeometry(const QPoint &pos);
    void setCursorIcon();

    bool m_resizable = false;

    MouseArea m_currentArea = Client;
    QPoint m_startPos;
    QPoint m_oldPos;
    QSize m_oldSize;
    int pan = 8;
};

#endif // DRAGGABLEFRAMELESSWINDOW_H
