#pragma once

#include <QObject>

#include <QStringList>

#include <QNetworkReply>

class MainWindow;

class PlayersChooser: public QObject {
	Q_OBJECT

public:
	enum State {
		Done,
		GettingPlayers,
		Error,
		ChoosingPlayer
	};

	PlayersChooser(MainWindow* window, QObject* parent = nullptr);
	~PlayersChooser() = default;

public slots:
	void choosePlayersRequested();

private slots:
	void playerListDownloadProgress(qint64 read, qint64 total);
	void playerListDownloaded();

private:
	void startDownloadPlayerList(const QUrl& url);
	void getRandomPlayers();

	QNetworkAccessManager* m_networkManager{};
	QNetworkReply* m_playerListReply{};

	QStringList m_availablePlayers{};

	MainWindow* m_window{};
};