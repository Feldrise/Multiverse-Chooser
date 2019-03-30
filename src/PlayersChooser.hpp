#include <QObject>

#include <QList>

#include <QNetworkReply>

class PlayersChooser: public QObject {
	Q_OBJECT

public:
	enum State {
		Done,
		GettingPlayers,
		Error,
		ChoosingPlayer
	};

	PlayersChooser(QObject* parent = nullptr);
	~PlayersChooser() = default;

private slots:
	void playerListDownloaded();

private:
	void startDownloadPlayerList(const QUrl& url);

	QNetworkAccessManager* m_networkManager{};
	QNetworkReply* m_playerListReply{};

	QList<QString> m_availablePlayers{};
};