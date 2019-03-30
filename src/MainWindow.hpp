#pragma once

#include <QWidget>

#include <QVBoxLayout>

#include <QLabel>
#include <QTextBrowser>
#include <QPushButton>

#include "PlayersChooser.hpp"

class MainWindow: public QWidget {
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow() = default;

	void setState(const PlayersChooser::State& state, const QString& message);
	void showChoosedPlayers(const QStringList& players) const;

private slots:
	void choosePlayersClicked();

signals:
	void choosePlayersRequested();

private:
	void setupUi();
	void loadSkin();

	QVBoxLayout* m_layout{};

	QLabel* m_description{};
	QTextBrowser* m_pseudos{};
	QLabel* m_stateLabel{};
	QPushButton* m_launchButton{};
};