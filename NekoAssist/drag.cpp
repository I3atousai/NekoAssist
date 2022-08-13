#include "drag.h"

int drag::m_inst = 0;

//image from internet
drag::drag(QLabel* pwgt, QString link, QString format) : m_format(format)
{
    ++m_inst;
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
                     this, SLOT (ShowContextMenu(const QPoint &)));

    QNetworkAccessManager* m_pnam;
    QNetworkRequest* req = new QNetworkRequest(QUrl(link));
    m_pnam = new QNetworkAccessManager(this);
    m_pnr = m_pnam->get(*req);

    m_path = QDir::tempPath() + randomString() + format;

    //background and size set in readDownFile()
    connect(m_pnr, SIGNAL(finished()), this, SLOT(readDownFile()));
}

//image from hard drive
drag::drag(QLabel* pwgt, QPixmap pix, QString format ) : QLabel (pwgt,  Qt::FramelessWindowHint),  m_pix(pix), m_format(format)
{
    ++m_inst;
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
                     this, SLOT (ShowContextMenu(const QPoint &)));

    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setPixmap(m_pix.scaled(75, 75, Qt::IgnoreAspectRatio, Qt::FastTransformation));

    this->show();
}

//text constructor
drag::drag(QLabel* pwgt, QString tex) : QLabel (pwgt), m_text(tex) {
    ++m_inst;
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
                     this, SLOT (ShowContextMenu(const QPoint &)));

    this->setWordWrap(true);
    this->setAlignment( Qt::AlignTop);
    this->setText(m_text);
    this->resize(75, 75);
    this->show();
}

void drag::startDragImg() {
    QMimeData* pMimeData = new QMimeData;
    QDrag* pDrag = new QDrag(this);

    QImage m_img;
    m_img.load(m_ba);
    if(m_ba.isEmpty())
        m_img = m_pix.toImage();
    pMimeData->setImageData(m_img);
    m_path = QDir::tempPath() + randomString() + m_format;
    m_img.save(m_path);
    pMimeData->setUrls(QList<QUrl>() << QUrl::fromLocalFile(m_path));
    pDrag->setMimeData(pMimeData);
    pDrag->setPixmap(m_pix.scaled(50,50));

    pDrag->exec(Qt::CopyAction | Qt::MoveAction);
}

void drag::startDragText() {
    QMimeData* pMimeData = new QMimeData;
    QDrag* pDrag = new QDrag(this);
    pMimeData->setText(m_text);
    pDrag->setMimeData(pMimeData);

    pDrag->exec(Qt::CopyAction | Qt::MoveAction);
}

void drag::mousePressEvent(QMouseEvent *pe)
{
     if (pe->button() == Qt::LeftButton) {
        m_ptPosition = pe->pos();
     }
}

void drag::mouseMoveEvent(QMouseEvent *pe) {
    if (pe->buttons() & Qt::LeftButton) {
        if(((pe->pos() - m_ptPosition).manhattanLength() > QApplication::startDragDistance()) & (!m_pix.isNull())) {
            startDragImg();
          }
        else if ((pe->pos() - m_ptPosition).manhattanLength() > QApplication::startDragDistance()) {
            startDragText();
        }
    }
}

QString drag::randomString () {
    srand(QTime::currentTime().msec());
    int num = rand();
    QString res = QString::number(num);

    return res;
}

void drag::enterEvent(QEnterEvent *pe) {
    this->resize(100,100);
    if(!m_pix.isNull()){
        this->setPixmap(m_pix.scaled(100,100, Qt::IgnoreAspectRatio, Qt::FastTransformation));
    }

}

void drag::leaveEvent(QEvent *pe) {
    this->resize(75,75);
    if(!m_pix.isNull()) {
        this->setPixmap(m_pix.scaled(75,75, Qt::IgnoreAspectRatio, Qt::FastTransformation));
    }

}

//void drag::deleteImg()
//{
//    //delete created file if exists
//    if(!m_pix.isNull()) {
//        QFile file(m_path);
//        file.remove();
//    }

//}

void drag::readDownFile() {
    QFile file(m_path);
    m_ba = m_pnr->readAll();
    if(file.open(QIODevice::WriteOnly)){
        file.write(m_ba);
        file.close();
    }

    m_pix.load(m_path);
    this->setPixmap(m_pix.scaled(75,75, Qt::IgnoreAspectRatio, Qt::FastTransformation));

}
void drag::ShowContextMenu(const QPoint &pos)
{
    QMenu conmenu(" ", this );
    QAction act1("Delete", this);
    //connect(&act1, SIGNAL(triggered()), this, SLOT(deleteImg()));
    connect(&act1, SIGNAL(triggered()), this, SLOT(deleteLater()));
    conmenu.addAction(&act1);
    conmenu.exec(mapToGlobal(pos));
}
drag::~drag () {
    //decrement the counter
    m_inst--;
}
