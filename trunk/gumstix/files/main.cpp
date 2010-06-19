#include <QCoreApplication>
#include <QDebug>

#include "rayaairbot.h"

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);

        RayaAirBot rayaairbot;

        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

	return app.exec();
}

