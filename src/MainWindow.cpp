#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget* parent) :
	QWidget(parent)
{
	// Some attributes
	setWindowTitle(tr("Multiverse Chooser"));
	setAttribute(Qt::WA_DeleteOnClose);

	// UI things
	setupUi();
	show();

	// Connections
	connect(m_launchButton, &QPushButton::clicked, this, &MainWindow::choosePlayersClicked);
}

void MainWindow::setState(PlayersChooser::State& state, QString& message) {}

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
	m_description = new QLabel(tr("Le multiverse à besoin de se rééquilibrer !"), this);

	m_pseudos = new QTextBrowser(this);
	m_pseudos->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	m_stateLabel = new QLabel(this);
	m_launchButton = new QPushButton(tr("REEQUILIBRAGE"));

	// Add widget into the layout
	m_layout->addWidget(m_description);
	m_layout->addWidget(m_pseudos);
	m_layout->addWidget(m_stateLabel);
	m_layout->addWidget(m_launchButton);
}
