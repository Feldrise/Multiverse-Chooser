#include <PlayersChooser.hpp>

#include <QMessageBox>

PlayersChooser::PlayersChooser(QObject* parent) :
	QObject(parent),
	m_networkManager(new QNetworkAccessManager(this))
{
	// TODO: remove
	QUrl playersUrl{QUrl("http://feldrise.com/multiverse/players.txt")};

	startDownloadPlayerList(playersUrl);
}

void PlayersChooser::playerListDownloaded()
{
	// Getting players info
	QByteArray playersInfo{m_playerListReply->readAll()};
	QTextStream in{&playersInfo};

	// Check errors
	if (m_playerListReply->error() != QNetworkReply::NoError)
		return;

	// Clear the list and add players into it
	m_availablePlayers.clear();

	while (!in.atEnd())
		m_availablePlayers.append(in.readLine());

	// TODO: remove
	QString playerList{};
	for(QString& player : m_availablePlayers) 
		playerList += player + ", ";
	QMessageBox::information(nullptr, "DEBUG", "Players: " + playerList);

	// Free the reply
	m_playerListReply->deleteLater();
}

void PlayersChooser::startDownloadPlayerList(const QUrl& url)
{
	// Download player list
	m_playerListReply = m_networkManager->get(QNetworkRequest(url));

	// Connect reply to action when downloaded
	connect(m_playerListReply, &QNetworkReply::finished, this, &PlayersChooser::playerListDownloaded);
}
