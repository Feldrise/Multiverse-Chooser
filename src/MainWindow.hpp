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

#pragma once

#include <QWidget>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QLabel>
#include <QTextBrowser>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox> 

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
	void switchTheme();
	void changeLanguage(const QString& lang);

signals:
	void choosePlayersRequested();

private:
	void setupUi();
	void loadSettings();
	void loadSkin(const QString& name);

	void translateApp();
	QString createLanguageItem(const QString& lang);
	QString currentLanguage() const;

	QVBoxLayout* m_layout{};
	QHBoxLayout* m_settingsLayout{};

	QLabel* m_description{};
	QTextBrowser* m_pseudos{};
	QLabel* m_stateLabel{};
	QPushButton* m_launchButton{};

	QCheckBox* m_whiteTheme{};
	QComboBox* m_lang{};

	QString m_languageFile{};
};