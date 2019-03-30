#include <QApplication>

#include <QWidget>

#include "PlayersChooser.hpp"

int main(int argc, char** argv)
{
	QApplication app{argc, argv};

	PlayersChooser* playerChooser{new PlayersChooser()};

	QWidget* window{new QWidget()};

	window->show();

	return app.exec();
}