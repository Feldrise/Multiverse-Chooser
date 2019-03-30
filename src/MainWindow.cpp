#include "MainWindow.hpp"

#include <QApplication>
#include <QMessageBox>

#include <QDir>

MainWindow::MainWindow(QWidget* parent) :
	QWidget(parent)
{
	// Some attributes
	setWindowTitle(tr("Multiverse Chooser"));
	setAttribute(Qt::WA_DeleteOnClose);

	// UI things
	setupUi();
	loadSkin();
	show();

	// Connections
	connect(m_launchButton, &QPushButton::clicked, this, &MainWindow::choosePlayersClicked);
}

void MainWindow::setState(const PlayersChooser::State& state, const QString& message)
{
	// Check if the state need specific ations
	if (state != PlayersChooser::Done && state != PlayersChooser::Error)
		m_launchButton->setEnabled(false);
	else
		m_launchButton->setEnabled(true);

	// Handle errors
	if (state == PlayersChooser::Error) {
		QMessageBox::critical(this, tr("Error"), message);
		m_stateLabel->setText(tr("An error occured!"));

		return;
	}

	// Update the state label
	m_stateLabel->setText(message);
}

void MainWindow::showChoosedPlayers(const QStringList& players) const
{
	QString playersText{"Players: \n"};

	// Add pseudos into the text
	for (const QString& player : players)
		playersText += QString(" - " + player + "\n");

	// Show pseudos
	m_pseudos->setText(playersText);
}

void MainWindow::choosePlayersClicked()
{
	// Emit the signal
	emit choosePlayersRequested();
}

void MainWindow::setupUi()
{
	// Resize the window
	resize(732, 524);

	m_layout = new QVBoxLayout(this);

	// Initialise widgets
	m_description = new QLabel(tr("The mutiverse need to be rebalance!"), this);

	m_pseudos = new QTextBrowser(this);
	m_pseudos->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	m_stateLabel = new QLabel(this);
	m_launchButton = new QPushButton(tr("REBALANCE"));

	// Add widget into the layout
	m_layout->addWidget(m_description);
	m_layout->addWidget(m_pseudos);
	m_layout->addWidget(m_stateLabel);
	m_layout->addWidget(m_launchButton);
}

void MainWindow::loadSkin()
{
	// Load skin file
	const QString fileName{"default.qss"};
	QString path{QApplication::applicationDirPath() + QDir::separator() + "skins" + QDir::separator() + fileName};

	// Load text in the file
	if (!path.isEmpty()) {
		QFile file{path};

		// Open the file
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QString style{file.readAll()};
			file.close(); // Close the file

			// Apply the skin
			(dynamic_cast<QApplication *>(QCoreApplication::instance()))->setStyleSheet(style);
		}
		else
			QMessageBox::warning(this, tr("Error"), tr("Unable to open skin file !"));
	}
}
