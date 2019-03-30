#include <QApplication>

#include <QWidget>

#include "MainWindow.hpp"
#include "PlayersChooser.hpp"

int main(int argc, char** argv)
{
	QApplication app{argc, argv};

	MainWindow* window{new MainWindow()};
	PlayersChooser* playerChooser{new PlayersChooser(window)};

	return app.exec();
}