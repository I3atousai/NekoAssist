#include "drop.h"
#include "drag.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    drop* DREG = new drop;
    DREG->show();

    return app.exec();
}

