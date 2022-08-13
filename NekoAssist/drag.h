#ifndef DRAG_H
#define DRAG_H

#include <QtWidgets>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class drag : public QLabel
{
    Q_OBJECT
public:
    //number of instances of an object
    static int m_inst;

    //constructor for images from internet
    explicit drag(QLabel* pwgt, QString link, QString format);

    //constructor for images from hard drive
    explicit drag(QLabel* pwgt, QPixmap pix, QString format);

    //text constructor
    explicit drag(QLabel* pwgt, QString tex);
    ~drag();

private:
    QPoint m_ptPosition;

    void startDragImg();
    QString m_format;
    QByteArray m_ba;
    QString m_path;
    QPixmap m_pix;
    QNetworkReply* m_pnr;

    void startDragText();
    QString m_text;

    //random file name generator to avoid name collasion for files from internet
    QString randomString();

    void mousePressEvent(QMouseEvent* pe) override;
    void mouseMoveEvent(QMouseEvent* pe) override;
    void enterEvent(QEnterEvent *pe) override;
    void leaveEvent(QEvent* pe) override;


private slots:
  //  void deleteImg();
    void ShowContextMenu(const QPoint &pos);
    void readDownFile();
};

#endif // DRAG_H
