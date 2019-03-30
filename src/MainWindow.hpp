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

	void setState(PlayersChooser::State& state, QString& message);

private slots:
	void choosePlayersClicked();

signals:
	void choosePlayersRequested();

private:
	void setupUi();

	QVBoxLayout* m_layout{};

	QLabel* m_description{};
	QTextBrowser* m_pseudos{};
	QLabel* m_stateLabel{};
	QPushButton* m_launchButton{};
};