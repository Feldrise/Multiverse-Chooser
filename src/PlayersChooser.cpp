#include <PlayersChooser.hpp>

#include <random>

#include "MainWindow.hpp"

PlayersChooser::PlayersChooser(MainWindow* window, QObject* parent) :
	QObject(parent),
	m_networkManager(new QNetworkAccessManager(this)),
	m_window(window)
{
	connect(m_window, &MainWindow::choosePlayersRequested, this, &PlayersChooser::choosePlayersRequested);
}

void PlayersChooser::choosePlayersRequested()
{
	// Download players list;
	QUrl playersUrl{QUrl("http://feldrise.com/multiverse/players.txt")};
	startDownloadPlayerList(playersUrl);

	// Update Window state
	m_window->setState(GettingPlayers, tr("Getting players list..."));
}

void PlayersChooser::playerListDownloadProgress(qint64 read, qint64 total)
{
	// Update Window state with percent info
	m_window->setState(GettingPlayers, tr("Getting players list (%1)").arg(QString::number((read / total) * 100)));
}

void PlayersChooser::playerListDownloaded()
{
	// Getting players info
	QByteArray playersInfo{m_playerListReply->readAll()};
	QTextStream in{&playersInfo};

	// Check errors
	if (m_playerListReply->error() != QNetworkReply::NoError) {
		m_window->setState(Error, tr("Error: %1").arg(m_playerListReply->errorString()));

		return;
	}

	// Clear the list and add players into it
	m_availablePlayers.clear();

	while (!in.atEnd())
		m_availablePlayers.append(in.readLine());

	// Free the reply
	m_playerListReply->deleteLater();

	getRandomPlayers();
}

void PlayersChooser::startDownloadPlayerList(const QUrl& url)
{
	// Download player list
	m_playerListReply = m_networkManager->get(QNetworkRequest(url));

	// Connect reply to some actions
	connect(m_playerListReply, &QNetworkReply::downloadProgress, this, &PlayersChooser::playerListDownloadProgress);
	connect(m_playerListReply, &QNetworkReply::finished, this, &PlayersChooser::playerListDownloaded);
}

void PlayersChooser::getRandomPlayers()
{
	// Shuffle the players list
	std::random_device randomDevice{};
	std::mt19937 g{randomDevice()};
	std::shuffle(m_availablePlayers.begin(), m_availablePlayers.end(), g);

	// Take the third players in the list
	QStringList choosedPlayers{};

	for (int i{0}; i < 3; ++i)
		choosedPlayers.append(m_availablePlayers[i]);

	// Show them on the window
	m_window->setState(Done, tr("You're all done!"));
	m_window->showChoosedPlayers(choosedPlayers);
}
