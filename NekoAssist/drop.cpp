#include "drop.h"

#define IS_IMG_FORMAT ( m_path.endsWith(".png") | m_path.endsWith(".jpg") | m_path.endsWith(".jpeg") | m_path.endsWith(".bmp"))

drop::drop(QWidget* pwgt): QLabel (pwgt,  Qt::FramelessWindowHint | Qt::Window | Qt::WindowStaysOnTopHint)
{   
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    this->setAttribute(Qt::WA_TranslucentBackground);
    m_backgr.load(":/images/Neco-Arc_Remake.png");
    this->setPixmap(m_backgr.scaled(170,250, Qt::IgnoreAspectRatio, Qt::FastTransformation));
    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
                     this, SLOT (ShowContextMenu(const QPoint &)));

    setAcceptDrops(true);
    m_lay = new QGridLayout();
    m_grid_x = 0;
    m_grid_y = 0;
    //to give user some space to grab the widget
    m_lay->setRowStretch(2,60);
    //want to make it changable from user interface
    max_number_of_files = 6;

    this->setLayout(m_lay);
}

void drop::ShowContextMenu(const QPoint &pos)
{
    QMenu conmenu(" ", this );
    QAction act1("Exit", this);
    QAction act2("Hide", this);
    connect(&act1, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(&act2, SIGNAL(triggered()), this, SLOT(showMinimized()));
    conmenu.addAction(&act1);
    conmenu.addAction(&act2);
    conmenu.exec(mapToGlobal(pos));
}

void drop::dragEnterEvent(QDragEnterEvent *pe)
{
  if(((drag::m_inst <= max_number_of_files)) & (!pe->mimeData()->hasImage()))
      pe->acceptProposedAction();
}

void drop::dropEvent(QDropEvent *pe)
{
    if(pe->mimeData()->hasUrls()) {
        m_path = pe->mimeData()->data("text/uri-list");
        m_path.remove(0,8);
        //removing '\n' from the end
        m_path.remove(m_path.size()-2, m_path.size());
    }
    if (pe->mimeData()->hasHtml()) {
        //separating data link from the whole html data
        m_path = pe->mimeData()->html();
        int ind = m_path.indexOf("src=", 0);
        m_path.remove(0, ind+5);
        ind = m_path.indexOf("\"");
        m_path.remove(ind, m_path.size());
    }
    //to check image fromat
    QString ends = "." + m_path.section('.', -1);

    //to place the child widgets at the 2nd row when first row if full
    if(drag::m_inst < max_number_of_files) {
         if(drag::m_inst == max_number_of_files/2){
             m_grid_y = 1;
             m_grid_x = 0;
         }
     }

    //for images from internet
     if (pe->mimeData()->hasHtml()){
         if(IS_IMG_FORMAT) {
             drag* var = new drag(this, m_path, ends);
             m_lay->addWidget(var, m_grid_y, m_grid_x);
             m_grid_x++;
         }
         else {
             //if cant detect image format default is PNG
             drag* var = new drag(this, m_path, ".png");
             m_lay->addWidget(var, m_grid_y, m_grid_x);
             m_grid_x++;
         }
     }
     //for images from hard drive
     else if (IS_IMG_FORMAT) {
         QPixmap pix;
         pix.load(m_path);
         drag* var = new drag(this, pix, ends);
         m_lay->addWidget(var, m_grid_y, m_grid_x);
         m_grid_x++;
     }
     //for text
     else {
         drag* var = new drag(this, pe->mimeData()->text());
         m_lay->addWidget(var, m_grid_y, m_grid_x);
         m_grid_x++;
     }
 }

//to drag the main widget around
void drop::mousePressEvent(QMouseEvent *pe) {
    m_ptPosition = pe->pos();
    if (pe->button() == Qt::RightButton)
        emit customContextMenuRequested(pe->pos());
}

void drop::mouseMoveEvent(QMouseEvent *pe) {

    move(pe->globalPosition().x()-m_ptPosition.x(), pe->globalPosition().y()-m_ptPosition.y());

}
