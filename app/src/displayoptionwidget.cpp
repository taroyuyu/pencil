/*

Pencil - Traditional Animation Software
Copyright (C) 2012-2020 Matthew Chiawen Chang

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

#include "displayoptionwidget.h"
#include "ui_displayoption.h"

#include <QToolButton>
#include <QGridLayout>

#include "preferencemanager.h"
#include "viewmanager.h"
#include "layermanager.h"
#include "scribblearea.h"
#include "editor.h"
#include "util.h"
#include "flowlayout.h"


DisplayOptionWidget::DisplayOptionWidget(QWidget *parent) :
    BaseDockWidget(parent),
    ui(new Ui::DisplayOption)
{
    ui->setupUi(this);
}

DisplayOptionWidget::~DisplayOptionWidget()
{
    delete ui;
}

void DisplayOptionWidget::initUI()
{
    updateUI();
    makeConnections();

    FlowLayout *layout = new FlowLayout;
    layout->setAlignment(Qt::AlignHCenter);
    layout->addWidget(ui->mirrorButton);
    layout->addWidget(ui->mirrorVButton);
    layout->addWidget(ui->thinLinesButton);
    layout->addWidget(ui->outLinesButton);
    layout->addWidget(ui->overlayCenterButton);
    layout->addWidget(ui->overlayThirdsButton);
    layout->addWidget(ui->overlayGoldenRatioButton);
    layout->addWidget(ui->overlaySafeAreaButton);

    delete ui->scrollAreaWidgetContents->layout();
    ui->scrollAreaWidgetContents->setLayout(layout);

#ifdef __APPLE__
    // Mac only style. ToolButtons are naturally borderless on Win/Linux.
    QString stylesheet =
        "QToolButton { border: 0px; } "
        "QToolButton:pressed{ border: 1px solid #FFADAD; border-radius: 2px; background-color: #D5D5D5; }"
        "QToolButton:checked{ border: 1px solid #ADADAD; border-radius: 2px; background-color: #D5D5D5; }";

    ui->mirrorButton->setStyleSheet(stylesheet);
    ui->mirrorVButton->setStyleSheet(stylesheet);
    ui->thinLinesButton->setStyleSheet(stylesheet);
    ui->outLinesButton->setStyleSheet(stylesheet);
    ui->overlayCenterButton->setStyleSheet(stylesheet);
    ui->overlayThirdsButton->setStyleSheet(stylesheet);
    ui->overlayGoldenRatioButton->setStyleSheet(stylesheet);
    ui->overlaySafeAreaButton->setStyleSheet(stylesheet);
#endif
}

void DisplayOptionWidget::makeConnections()
{    
    connect(ui->mirrorButton, &QToolButton::clicked, this, &DisplayOptionWidget::toggleMirror);
    connect(ui->mirrorVButton, &QToolButton::clicked, this, &DisplayOptionWidget::toggleMirrorV);
    connect(ui->overlayCenterButton, &QToolButton::clicked, this, &DisplayOptionWidget::toggleOverlayCenter);
    connect(ui->overlayThirdsButton, &QToolButton::clicked, this, &DisplayOptionWidget::toggleOverlayThirds);
    connect(ui->overlayGoldenRatioButton, &QToolButton::clicked, this, &DisplayOptionWidget::toggleOverlayGoldenRatio);
    connect(ui->overlaySafeAreaButton, &QToolButton::clicked, this, &DisplayOptionWidget::toggleOverlaySafeAreas);

    PreferenceManager* prefs = editor()->preference();
    ScribbleArea* pScriArea = editor()->getScribbleArea();

    connect(ui->thinLinesButton, &QToolButton::clicked, pScriArea, &ScribbleArea::toggleThinLines);
    connect(ui->outLinesButton, &QToolButton::clicked, pScriArea, &ScribbleArea::toggleOutlines);
    connect(prefs, &PreferenceManager::optionChanged, this, &DisplayOptionWidget::updateUI);

    ViewManager* view = editor()->view();
    connect(view, &ViewManager::viewFlipped, this, &DisplayOptionWidget::updateUI);
}

void DisplayOptionWidget::updateUI()
{
    PreferenceManager* prefs = editor()->preference();

    bool canEnableVectorButtons = editor()->layers()->currentLayer()->type() == Layer::VECTOR;
    ui->thinLinesButton->setEnabled(canEnableVectorButtons);
    ui->outLinesButton->setEnabled(canEnableVectorButtons);

    QSignalBlocker b1(ui->thinLinesButton);
    ui->thinLinesButton->setChecked(prefs->isOn(SETTING::INVISIBLE_LINES));

    QSignalBlocker b2(ui->outLinesButton);
    ui->outLinesButton->setChecked(prefs->isOn(SETTING::OUTLINES));

    QSignalBlocker b9(ui->overlayCenterButton);
    ui->overlayCenterButton->setChecked(prefs->isOn(SETTING::OVERLAY_CENTER));

    QSignalBlocker b10(ui->overlayThirdsButton);
    ui->overlayThirdsButton->setChecked(prefs->isOn(SETTING::OVERLAY_THIRDS));

    QSignalBlocker b11(ui->overlayGoldenRatioButton);
    ui->overlayGoldenRatioButton->setChecked(prefs->isOn(SETTING::OVERLAY_GOLDEN));

    QSignalBlocker b12(ui->overlaySafeAreaButton);
    ui->overlaySafeAreaButton->setChecked(prefs->isOn(SETTING::OVERLAY_SAFE));

    if (prefs->isOn(SETTING::ACTION_SAFE_ON) || prefs->isOn(SETTING::TITLE_SAFE_ON))
    {
        ui->overlaySafeAreaButton->setEnabled(true);
    } else {
        ui->overlaySafeAreaButton->setEnabled(false);
    }

    ViewManager* view = editor()->view();

    QSignalBlocker b3(ui->mirrorButton);
    ui->mirrorButton->setChecked(view->isFlipHorizontal());

    QSignalBlocker b4(ui->mirrorVButton);
    ui->mirrorVButton->setChecked(view->isFlipVertical());
}

int DisplayOptionWidget::getMinHeightForWidth(int width)
{
    return ui->innerWidget->layout()->heightForWidth(width);
}

void DisplayOptionWidget::toggleMirror(bool isOn)
{
    editor()->view()->flipHorizontal(isOn);
}

void DisplayOptionWidget::toggleMirrorV(bool isOn)
{
    editor()->view()->flipVertical(isOn);
}

void DisplayOptionWidget::toggleOverlayCenter(bool isOn)
{
    editor()->view()->setOverlayCenter(isOn);
    editor()->preference()->set(SETTING::OVERLAY_CENTER, isOn);
}

void DisplayOptionWidget::toggleOverlayThirds(bool isOn)
{
    editor()->view()->setOverlayThirds(isOn);
    editor()->preference()->set(SETTING::OVERLAY_THIRDS, isOn);
}

void DisplayOptionWidget::toggleOverlayGoldenRatio(bool isOn)
{
    editor()->view()->setOverlayGoldenRatio(isOn);
    editor()->preference()->set(SETTING::OVERLAY_GOLDEN, isOn);
}

void DisplayOptionWidget::toggleOverlaySafeAreas(bool isOn)
{
    editor()->view()->setOverlaySafeAreas(isOn);
    editor()->preference()->set(SETTING::OVERLAY_SAFE, isOn);
}
