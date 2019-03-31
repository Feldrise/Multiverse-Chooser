/***********************************************************************************
** MIT License                                                                    **
**                                                                                **
** Copyright (c) 2019 Victor DENIS (victordenis01@gmail.com)                      **
**                                                                                **
** Permission is hereby granted, free of charge, to any person obtaining a copy   **
** of this software and associated documentation files (the "Software"), to deal  **
** in the Software without restriction, including without limitation the rights   **
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      **
** copies of the Software, and to permit persons to whom the Software is          **
** furnished to do so, subject to the following conditions:                       **
**                                                                                **
** The above copyright notice and this permission notice shall be included in all **
** copies or substantial portions of the Software.                                **
**                                                                                **
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     **
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       **
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    **
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         **
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  **
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  **
** SOFTWARE.                                                                      **
***********************************************************************************/

#include "MainWindow.hpp"

#include <QApplication>
#include <QTranslator>
#include <QSettings>

#include <QMessageBox>

#include <QDir>
#include <QLibraryInfo>

MainWindow::MainWindow(QWidget* parent) :
	QWidget(parent)
{
	// Some attributes
	setWindowTitle(tr("Multiverse Chooser"));
	setAttribute(Qt::WA_DeleteOnClose);

	// Translation 
	translateApp();

	// UI things
	setupUi();
	show();

	// Load settings
	loadSettings();

	// Connections
	connect(m_whiteTheme, &QCheckBox::stateChanged, this, &MainWindow::switchTheme);
	connect(m_launchButton, &QPushButton::clicked, this, &MainWindow::choosePlayersClicked);
	connect(m_lang, &QComboBox::currentTextChanged, this, &MainWindow::changeLanguage);
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

void MainWindow::switchTheme()
{
	// Get theme
	QString name{"default"};

	if (m_whiteTheme->isChecked())
		name = "white";

	// Save currentTheme
	QSettings settings{};
	settings.setValue("currentTheme", name);

	// Load theme
	loadSkin(name);
}

void MainWindow::changeLanguage(const QString& lang)
{
	QSettings settings{};
	settings.setValue("Language/language", lang);

	QMessageBox::information(this, tr("Information"), tr("You may need to restart the app!"));

	translateApp();
}

void MainWindow::setupUi()
{
	// Resize the window
	resize(366, 262);

	m_layout = new QVBoxLayout(this);
	m_settingsLayout = new QHBoxLayout();

	// Initialise widgets
	m_description = new QLabel(tr("The mutiverse need to be rebalanced!"), this);

	m_pseudos = new QTextBrowser(this);
	m_pseudos->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	m_stateLabel = new QLabel(this);
	m_launchButton = new QPushButton(tr("REBALANCE"));

	m_whiteTheme = new QCheckBox(tr("White Theme"), this);
	m_lang = new QComboBox(this);

	// Add widget into the layout
	m_settingsLayout->addWidget(m_whiteTheme);
	m_settingsLayout->addWidget(m_lang);

	m_layout->addWidget(m_description);
	m_layout->addLayout(m_settingsLayout);
	m_layout->addWidget(m_pseudos);
	m_layout->addWidget(m_stateLabel);
	m_layout->addWidget(m_launchButton);
}

void MainWindow::loadSettings()
{
	QSettings settings{};

	// Theme settings
	const QString themeName = settings.value("currentTheme", "default").toString();

	loadSkin(themeName);

	// Translation settings
	settings.beginGroup("Language");

	QString activeLanguage = currentLanguage();

	if (!activeLanguage.isEmpty() && activeLanguage != QLatin1String("en_US"))
		m_lang->addItem(createLanguageItem(activeLanguage), activeLanguage);

	m_lang->addItem("English (en_US)", "en_US");

	QString translationPath{QApplication::applicationDirPath() + QDir::separator() + "locale"};
	QDir lanDir{translationPath};
	QStringList list = lanDir.entryList(QStringList("*.qm"));

	foreach(const QString& name, list)
	{
		if (name.startsWith(QLatin1String("qt_")))
			continue;

		QString loc{name};
		loc.remove(QLatin1String(".qm"));

		if (loc == activeLanguage)
			continue;

		m_lang->addItem(createLanguageItem(loc), loc);
	}

	settings.endGroup();
}

void MainWindow::loadSkin(const QString& name)
{
	// Load skin file
	const QString fileName{name  + ".qss"};
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

void MainWindow::translateApp()
{
	QSettings settings{};
	QString file{settings.value("Language/language", QLocale::system().name()).toString()};

	// It can only be "C" locale, for which we will use default English language
	if (file.size() < 2)
		file.clear();

	if (!file.isEmpty() && !file.endsWith(QLatin1String(".qm")))
		file.append(QLatin1String(".qm"));

	// Either we load default language (with empty file), or we attempt to load xx.qm (xx_yy.qm)
	Q_ASSERT(file.isEmpty() || file.size() >= 5);

	QString translationPath{QApplication::applicationDirPath() + QDir::separator() + "locale"};

	if (!file.isEmpty()) {
		if (!QFile(QString("%1/%2").arg(translationPath, file)).exists()) {
			QDir dir{translationPath};
			QString lang{file.left(2) + QLatin1String("*.qm")};

			const QStringList translations = dir.entryList(QStringList(lang));

			// If no translation can be found, we will use the default English
			file = translations.isEmpty() ? QString() : translations[0];
		}
	}

	// Application translations
	QTranslator* app{new QTranslator(this)};
	app->load(file, translationPath);

	// Qt translations
	QTranslator* sys{new QTranslator(this)};
	sys->load("qt_" + file, QLibraryInfo::location(QLibraryInfo::TranslationsPath));

	if (sys->isEmpty())
		sys->load("qt_" + file, translationPath);

	m_languageFile = file;

	qApp->installTranslator(app);
	qApp->installTranslator(sys);
}

QString MainWindow::createLanguageItem(const QString& lang)
{
	QLocale locale{lang};

	if (locale.language() == QLocale::C)
		return lang;

	QString country{QLocale::countryToString(locale.country())};
	QString language{QLocale::languageToString(locale.language())};

	return QString("%1, %2 (%3)").arg(language, country, lang);
}

QString MainWindow::currentLanguage() const
{
	QString lang = m_languageFile;

	if (lang.isEmpty()) {
		return "en_US";
	}

	return lang.left(lang.length() - 3);
}
