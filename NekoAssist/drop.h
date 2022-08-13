#ifndef DROP_H
#define DROP_H

#include "drag.h"

class drop : public QLabel
{
    Q_OBJECT
public:
    explicit drop(QWidget* pwgt  = nullptr);

 private:
     QPixmap m_backgr;
     QPoint m_ptDragPos;
     QPoint m_ptPosition;
     QString m_path;
     QGridLayout* m_lay;
     int m_grid_y;
     int m_grid_x;
     int max_number_of_files;


     void dragEnterEvent(QDragEnterEvent* pe) override;
     void dropEvent(QDropEvent* pe) override;

     void mousePressEvent(QMouseEvent* pe) override;
     void mouseMoveEvent(QMouseEvent* pe) override;

 public slots:
     void ShowContextMenu(const QPoint &pos);
};

#endif // DROP_H
