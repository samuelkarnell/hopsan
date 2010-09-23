/*
 * This file is part of OpenModelica.
 *
 * Copyright (c) 1998-CurrentYear, Linköping University,
 * Department of Computer and Information Science,
 * SE-58183 Linköping, Sweden.
 *
 * All rights reserved.
 *
 * THIS PROGRAM IS PROVIDED UNDER THE TERMS OF GPL VERSION 3 
 * AND THIS OSMC PUBLIC LICENSE (OSMC-PL). 
 * ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS PROGRAM CONSTITUTES RECIPIENT'S  
 * ACCEPTANCE OF THE OSMC PUBLIC LICENSE.
 *
 * The OpenModelica software and the Open Source Modelica
 * Consortium (OSMC) Public License (OSMC-PL) are obtained
 * from Linköping University, either from the above address,
 * from the URLs: http://www.ida.liu.se/projects/OpenModelica or  
 * http://www.openmodelica.org, and in the OpenModelica distribution. 
 * GNU version 3 is obtained from: http://www.gnu.org/copyleft/gpl.html.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without
 * even the implied warranty of  MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE, EXCEPT AS EXPRESSLY SET FORTH
 * IN THE BY RECIPIENT SELECTED SUBSIDIARY LICENSE CONDITIONS
 * OF OSMC-PL.
 *
 * See the full OSMC Public License conditions for more details.
 *
 */

/*
 * HopsanGUI
 * Fluid and Mechatronic Systems, Department of Management and Engineering, Linköping University
 * Main Authors 2009-2010:  Robert Braun, Björn Eriksson, Peter Nordin
 * Contributors 2009-2010:  Mikael Axin, Alessandro Dell'Amico, Karl Pettersson, Ingo Staack
 */

//$Id$

#include <QDebug>
#include <QSpinBox>
#include <QColorDialog>
#include <QLabel>
#include <QCursor>
#include <QAction>

#include "PlotWidget.h"

#include "MainWindow.h"
#include "ProjectTabWidget.h"
#include "GUIObject.h"
#include "GUIPort.h"
#include "GraphicsView.h"
#include "GUISystem.h"

#include "qwt_scale_engine.h"

#include "qwt_symbol.h"
#include "qwt_text_label.h"

PlotWindow::PlotWindow(QVector<double> xarray, QVector<double> yarray, VariableList *variableList, MainWindow *parent)
    : QMainWindow(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose);

    mpParentMainWindow = parent;
    mpCurrentGUISystem = mpParentMainWindow->mpProjectTabs->getCurrentSystem();
    mpVariableList = variableList;

    mHasSpecialXAxis = false;

        //Create the plot
    mpVariablePlot = new VariablePlot();
    mpVariablePlot->setAcceptDrops(false);

    nCurves = 0;
    mCurveColors << "Blue" << "Red" << "Green" << "Orange";

    mLeftAxisLogarithmic = false;
    mRightAxisLogarithmic = false;

        //Create mpToolBar and toolbutton
    mpToolBar = new QToolBar(this);
    mpToolBar->setAcceptDrops(false);

    mpZoomButton = new QToolButton(mpToolBar);
    mpZoomButton->setToolTip("Zoom");
    mpZoomButton->setIcon(QIcon(QString(ICONPATH) + "Hopsan-Zoom.png"));
    mpZoomButton->setCheckable(true);
    mpZoomButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mpZoomButton->setAcceptDrops(false);
    mpToolBar->addWidget(mpZoomButton);

    mpPanButton = new QToolButton(mpToolBar);
    mpPanButton->setToolTip("Pan");
    mpPanButton->setIcon(QIcon(QString(ICONPATH) + "Hopsan-Pan.png"));
    mpPanButton->setCheckable(true);
    mpPanButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mpPanButton->setAcceptDrops(false);
    mpToolBar->addWidget(mpPanButton);

    mpSVGButton = new QToolButton(mpToolBar);
    mpSVGButton->setToolTip("Export to SVG");
    mpSVGButton->setIcon(QIcon(QString(ICONPATH) + "Hopsan-SaveToSvg.png"));
    mpSVGButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mpSVGButton->setAcceptDrops(false);
    mpToolBar->addWidget(mpSVGButton);

    mpGNUPLOTButton = new QToolButton(mpToolBar);
    mpGNUPLOTButton->setToolTip("Export to GNUPLOT");
    mpGNUPLOTButton->setIcon(QIcon(QString(ICONPATH) + "Hopsan-SaveToGnuPlot.png"));
    mpGNUPLOTButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mpGNUPLOTButton->setAcceptDrops(false);
    mpToolBar->addWidget(mpGNUPLOTButton);

    mpGridButton = new QToolButton(mpToolBar);
    mpGridButton->setToolTip("Show Grid");
    mpGridButton->setIcon(QIcon(QString(ICONPATH) + "Hopsan-Grid.png"));
    mpGridButton->setCheckable(true);
    mpGridButton->setChecked(true);
    mpGridButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mpGridButton->setAcceptDrops(false);
    mpToolBar->addWidget(mpGridButton);

    mpColorButton = new QToolButton(mpToolBar);
    mpColorButton->setToolTip("Select Line Color");
    mpColorButton->setIcon(QIcon(QString(ICONPATH) + "Hopsan-LineColor.png"));
    mpColorButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mpColorButton->setAcceptDrops(false);
    mpToolBar->addWidget(mpColorButton);

    mpBackgroundColorButton = new QToolButton(mpToolBar);
    mpBackgroundColorButton->setToolTip("Select Canvas Color");
    mpBackgroundColorButton->setIcon(QIcon(QString(ICONPATH) + "Hopsan-BackgroundColor.png"));
    mpBackgroundColorButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mpBackgroundColorButton->setAcceptDrops(false);
    mpToolBar->addWidget(mpBackgroundColorButton);

    //mpSizeButton = new QmpToolBar(tr("Size Spinbox"));
    mpSizeLabel = new QLabel(tr("Line Width: "));
    mpSizeLabel->setAcceptDrops(false);
    mpSizeSpinBox = new QSpinBox(mpToolBar);
    mpSizeSpinBox->setAcceptDrops(false);
    //mpSizeButton->set("Line Width");
    mpSizeSpinBox->setRange(1,10);
    mpSizeSpinBox->setSingleStep(1);
    mpSizeSpinBox->setValue(2);
    mpSizeSpinBox->setSuffix(" pt");
    //mpSizeButton->setOrientation(Qt::Vertical);
    //mpSizeButton->addWidget(mpSizeLabel);
    //mpSizeButton->addWidget(mpSizeSpinBox);
    mpToolBar->addWidget(mpSizeLabel);
    mpToolBar->addWidget(mpSizeSpinBox);

    addToolBar(mpToolBar);

    //Zoom
    mpZoomer = new QwtPlotZoomer( QwtPlot::xBottom, QwtPlot::yLeft, mpVariablePlot->canvas());
    mpZoomer->setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);
    mpZoomer->setRubberBand(QwtPicker::RectRubberBand);
    mpZoomer->setRubberBandPen(QColor(Qt::green));
    mpZoomer->setTrackerMode(QwtPicker::ActiveOnly);
    mpZoomer->setTrackerPen(QColor(Qt::white));
    mpZoomer->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ControlModifier);
    mpZoomer->setMousePattern(QwtEventPattern::MouseSelect3, Qt::RightButton);

    //Panner
    mpPanner = new QwtPlotPanner(mpVariablePlot->canvas());
    mpPanner->setMouseButton(Qt::MidButton);

    //grid
    mpGrid = new QwtPlotGrid;
    mpGrid->enableXMin(true);
    mpGrid->enableYMin(true);
    mpGrid->setMajPen(QPen(Qt::black, 0, Qt::DotLine));
    mpGrid->setMinPen(QPen(Qt::gray, 0 , Qt::DotLine));
    mpGrid->attach(mpVariablePlot);
    //grid->hide();

        // Create and add curves to the plot
    tempCurve = new QwtPlotCurve();
    QwtArrayData data(xarray,yarray);
    tempCurve->setData(data);
    tempCurve->attach(mpVariablePlot);
    mpVariablePlot->setCurve(tempCurve);
    tempCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    mpVariablePlot->replot();
    tempCurve->setPen(QPen(QBrush(QColor(mCurveColors[nCurves])),mpSizeSpinBox->value()));
    mpCurves.append(tempCurve);
    ++nCurves;

        //Create the close button
    QDialogButtonBox *buttonbox = new QDialogButtonBox(QDialogButtonBox::Close);
    buttonbox->setAcceptDrops(false);

    this->setCentralWidget(mpVariablePlot);

    enableZoom(false);

    //Establish signal and slots connections
    connect(buttonbox, SIGNAL(rejected()), this, SLOT(close()));
    connect(mpZoomButton,SIGNAL(toggled(bool)),SLOT(enableZoom(bool)));
    connect(mpPanButton,SIGNAL(toggled(bool)),SLOT(enablePan(bool)));
    connect(mpSVGButton,SIGNAL(clicked()),SLOT(exportSVG()));
    connect(mpGNUPLOTButton,SIGNAL(clicked()),SLOT(exportGNUPLOT()));
    connect(mpGridButton,SIGNAL(toggled(bool)),SLOT(enableGrid(bool)));
    connect(mpSizeSpinBox,SIGNAL(valueChanged(int)),this, SLOT(setSize(int)));
    connect(mpColorButton,SIGNAL(clicked()),this,SLOT(setColor()));
    connect(mpBackgroundColorButton,SIGNAL(clicked()),this,SLOT(setBackgroundColor()));

    resize(600,600);

    this->setAcceptDrops(true);



    mpMarker = new QwtPlotMarker();
    mpMarkerSymbol = new QwtSymbol();
    mpMarkerSymbol->setBrush(QBrush(Qt::red, Qt::SolidPattern));
    mpMarkerSymbol->setStyle(QwtSymbol::Ellipse);
    mpMarkerSymbol->setSize(10,10);
    mpMarker->setSymbol(*mpMarkerSymbol);
    mpMarker->setXValue(0);
    mpMarker->setYValue(0);
    mpMarker->attach(mpVariablePlot);

    mpLabelText = new QwtText();
    mpLabelText->setText("(0.0, 0.0)");
    mpLabelText->setBackgroundBrush(QColor("yellow"));
    mpLabelText->setFont(QFont("Arial", 14));
    mpLabel = new QwtTextLabel(*mpLabelText, this);
    mpLabel->setGeometry(0, 0, 70, 24);
    mpLabel->adjustSize();
    mpLabel->show();
}

void PlotWindow::enableZoom(bool on)
{
    mpZoomer->setEnabled(on);
    //mpZoomer->zoom(0);

    mpPanner->setEnabled(on);
    mpPanner->setMouseButton(Qt::MidButton);

    disconnect(mpPanButton,SIGNAL(toggled(bool)),this,SLOT(enablePan(bool)));
    mpPanButton->setChecked(false);
    connect(mpPanButton,SIGNAL(toggled(bool)),this, SLOT(enablePan(bool)));
    mpPanner->setEnabled(false);
}


//! Slot that enables or disables
void PlotWindow::enablePan(bool on)
{
    mpPanner->setEnabled(on);
    mpPanner->setMouseButton(Qt::LeftButton);

    disconnect(mpZoomButton,SIGNAL(toggled(bool)),this,SLOT(enableZoom(bool)));
    mpZoomButton->setChecked(false);
    connect(mpZoomButton,SIGNAL(toggled(bool)),this,SLOT(enableZoom(bool)));
    mpZoomer->setEnabled(false);
}


//! Slot that turns plot grid on or off
void PlotWindow::enableGrid(bool on)
{
    if (on)
    {
        mpGrid->show();
    }
    else
    {
        mpGrid->hide();
    }

}


//! @Slot that exports current plot to .svg format
void PlotWindow::exportSVG()
{
#ifdef QT_SVG_LIB
#ifndef QT_NO_FILEDIALOG
     QString fileName = QFileDialog::getSaveFileName(
        this, "Export File Name", QString(),
        "SVG Documents (*.svg)");
#endif
    if ( !fileName.isEmpty() )
    {
        QSvgGenerator generator;
        generator.setFileName(fileName);
        generator.setSize(QSize(800, 600));
        mpVariablePlot->print(generator);
    }
#endif
}


//! Slot that exports a curve to GNUPLOT format
//! @todo Currently only the last added curve will be exported...
void PlotWindow::exportGNUPLOT()
{
    QDir fileDialogSaveDir;
    QString modelFileName = QFileDialog::getSaveFileName(this, tr("Save Model File"),
                                                         fileDialogSaveDir.currentPath(),
                                                         tr("GNUPLOT File (*.GNUPLOT)"));
    QFile file(modelFileName);
    QFileInfo fileInfo(file);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for writing: " + modelFileName;
        return;
    }
    QTextStream modelFile(&file);  //Create a QTextStream object to stream the content of file

    size_t size = mpVariablePlot->getCurve()->data().size();
    for(std::size_t i=0; i!=size; ++i)
    {
        modelFile << mpVariablePlot->getCurve()->data().x(i);
        modelFile << " ";
        modelFile << mpVariablePlot->getCurve()->data().y(i);
        modelFile << "\n";
    }
    file.close();
}

void PlotWindow::setSize(int size)
{
    for(size_t i=0; i<mpCurves.size(); ++i)
    {
        mpCurves.at(i)->setPen(QPen(mpCurves.at(i)->pen().color(),size));
    }
}

void PlotWindow::setColor()
{
    QMenu menu;


    //QVector<*QAction> curves;
    //QAction *tempAction;

    for(int i=0; i<mpCurves.size(); ++i)
    {
        menu.addAction(mpCurves[i]->title().text());
    }

    QCursor *cursor;
    QAction *selectedAction = menu.exec(cursor->pos());

    if(selectedAction == 0)
    {
        return;
    }

    QwtPlotCurve *pSelectedCurve;

    for(int i=0; i<mpCurves.size(); ++i)
    {
        if (selectedAction->text() == mpCurves[i]->title().text())
        {
            pSelectedCurve = mpCurves[i];
        }
    }

    QColor color = QColorDialog::getColor(pSelectedCurve->pen().color(), this);
    if (color.isValid())
    {
        pSelectedCurve->setPen(QPen(color, pSelectedCurve->pen().width()));
    }
}

void PlotWindow::setBackgroundColor()
{
    QColor color = QColorDialog::getColor(this->mpVariablePlot->canvasBackground(), this);
    if (color.isValid())
    {
        mpVariablePlot->setCanvasBackground(color);
        mpVariablePlot->replot();
    }
}


void PlotWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText())
    {
        mpHoverRect = new QRubberBand(QRubberBand::Rectangle,this);
        mpHoverRect->setGeometry(0, 0, this->width(), this->height());
        mpHoverRect->setWindowOpacity(1);
        mpHoverRect->show();

        event->acceptProposedAction();
    }
}

void PlotWindow::mouseMoveEvent(QMouseEvent *event)
{

    QCursor cursor;
    int correctionFactor = mpVariablePlot->canvas()->x()+5;
    int intX = this->mapFromGlobal(cursor.pos()).x() - correctionFactor;
    qDebug() << "intX " << intX;
    double x = mpVariablePlot->canvasMap(QwtPlot::xBottom).invTransform(intX);
    if(x < 0)
    {
        x = 0;
    }
    if(intX < 0)
    {
        intX = 0;
        qDebug() << "Outside!";
    }
    //int xDataPos = intX*mpCurves[0]->dataSize()/(mpVariablePlot->canvas()->width()-11)-4;
    int xDataPos = x/mpCurves[0]->maxXValue()*mpCurves[0]->dataSize();
    if(xDataPos > mpCurves[0]->dataSize()-1)
    {
        xDataPos = mpCurves[0]->dataSize()-1;
        qDebug() << "Outside!";
    }
    //qDebug() << "Moving mouse, dataSize = " << mpCurves[0]->dataSize() << ", xDataPos = " << xDataPos << ", x = " << x;
    double y = mpCurves[0]->y(std::max(0, xDataPos));
    mpMarker->setXValue(x);
    mpMarker->setYValue(y);

    QString xString;
    QString yString;
    xString.setNum(x);
    yString.setNum(y);

    mpLabelText->setText("("+xString+", "+yString+")");
    mpLabel->setText(*mpLabelText);
    mpLabel->setGeometry(mpVariablePlot->canvasMap(QwtPlot::xBottom).xTransform(x), mpVariablePlot->canvasMap(QwtPlot::yLeft).xTransform(y),0,0);
    mpLabel->adjustSize();

    mpVariablePlot->replot();
}


void PlotWindow::dragMoveEvent(QDragMoveEvent *event)
{
    QCursor cursor;
    if(this->mapFromGlobal(cursor.pos()).y() > this->height()/2 && mpCurves.size() >= 1)
    {
        mpHoverRect->setGeometry(mpVariablePlot->canvas()->x(), mpVariablePlot->canvas()->height()/2+mpVariablePlot->canvas()->y()+34, mpVariablePlot->canvas()->width(), mpVariablePlot->canvas()->height()/2);
    }
    else if(this->mapFromGlobal(cursor.pos()).x() < this->width()/2)
    {
        mpHoverRect->setGeometry(mpVariablePlot->canvas()->x(), mpVariablePlot->canvas()->y()+34, mpVariablePlot->canvas()->width()/2, mpVariablePlot->canvas()->height());
    }
    else
    {
        mpHoverRect->setGeometry(mpVariablePlot->canvas()->x() + mpVariablePlot->canvas()->width()/2, mpVariablePlot->canvas()->y()+34, mpVariablePlot->canvas()->width()/2, mpVariablePlot->canvas()->height());
    }
    QMainWindow::dragMoveEvent(event);
}


void PlotWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    delete(mpHoverRect);
    QMainWindow::dragLeaveEvent(event);
}


//! Defines what happens when something is dropped in a plot window
void PlotWindow::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasText())
    {
        delete(mpHoverRect);

        QString mimeText = event->mimeData()->text();

        if(mimeText.startsWith("HOPSANPLOTDATA"))
        {
            QString lookupName;
            lookupName = QString(mimeText.right(mimeText.size()-15));

            QString title;
            QString xlabel;
            QString ylabel;

            title.append(lookupName);
            ylabel.append(mpVariableList->yLabelMap.find(lookupName).value());
            xlabel.append("Time, [s]");

            QCursor cursor;
            if(this->mapFromGlobal(cursor.pos()).y() > this->height()/2 && mpCurves.size() >= 1)
            {
                this->changeXVector(mpVariableList->yMap.find(lookupName).value(), ylabel);
            }
            else if(this->mapFromGlobal(cursor.pos()).x() < this->width()/2)
            {
                this->addPlotCurve(mpVariableList->xMap.find(lookupName).value(),mpVariableList->yMap.find(lookupName).value(), title, xlabel, ylabel, QwtPlot::yLeft);
            }
            else
            {
                this->addPlotCurve(mpVariableList->xMap.find(lookupName).value(),mpVariableList->yMap.find(lookupName).value(), title, xlabel, ylabel, QwtPlot::yRight);
            }
        }
    }
}

void PlotWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;

    QMenu *yAxisRightMenu;
    QMenu *yAxisLeftMenu;
    //QAction *xAxisAction;

    yAxisRightMenu = menu.addMenu(QString("Right Y Axis"));
    yAxisLeftMenu = menu.addMenu(QString("Left Y Axis"));

    QAction *setRightAxisLogarithmic;
    QAction *setLeftAxisLogarithmic;

    setRightAxisLogarithmic = yAxisRightMenu->addAction("Logarithmic Scale");
    setLeftAxisLogarithmic = yAxisLeftMenu->addAction("Logarithmic Scale");

    setRightAxisLogarithmic->setCheckable(true);
    setLeftAxisLogarithmic->setCheckable(true);
    setRightAxisLogarithmic->setChecked(mRightAxisLogarithmic);
    setLeftAxisLogarithmic->setChecked(mLeftAxisLogarithmic);

    QCursor *cursor;
    QAction *selectedAction = menu.exec(cursor->pos());

    if (selectedAction == setRightAxisLogarithmic)
    {
        mRightAxisLogarithmic = !mRightAxisLogarithmic;
        if(mRightAxisLogarithmic)
        {
            mpVariablePlot->setAxisScaleEngine(QwtPlot::yRight, new QwtLog10ScaleEngine);
        }
        else
        {
            mpVariablePlot->setAxisScaleEngine(QwtPlot::yRight, new QwtLinearScaleEngine);
        }
    }
    else if (selectedAction == setLeftAxisLogarithmic)
    {
        mLeftAxisLogarithmic = !mLeftAxisLogarithmic;
        if(mLeftAxisLogarithmic)
        {
            mpVariablePlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLog10ScaleEngine);
        }
        else
        {
            mpVariablePlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine);
        }
    }
}


//! Help function to add a new curve to an existing plot window
//! @param xarray is the vector for the x-axis
//! @param yarray is the vector for the y-axis
//! @param title is the title of the curve
//! @param xLabel is the label for the x-axis
//! @param yLabel is the label for the y-axis
//! @param axisY tells whether the right or left y-axis shall be used
void PlotWindow::addPlotCurve(QVector<double> xarray, QVector<double> yarray, QString title, QString xLabel, QString yLabel, QwtPlot::Axis axisY)
{

        // Create and add curves to the plot
    tempCurve = new QwtPlotCurve(title);
    if(!mHasSpecialXAxis)
    {
        tempCurve->setData(xarray, yarray);
        mpVariablePlot->setAxisTitle(VariablePlot::xBottom, xLabel);
    }
    else
    {
        QVector<double> tempXarray;
        for(size_t j=0; j<mpCurves.last()->data().size(); ++j)
        {
            tempXarray.append(mpCurves.last()->data().x(j));
        }
        tempCurve->setData(tempXarray, yarray);
    }
    tempCurve->attach(mpVariablePlot);
    mpVariablePlot->setCurve(tempCurve);
    mpVariablePlot->enableAxis(axisY, true);
    tempCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    mpVariablePlot->replot();
    int size = mpSizeSpinBox->value();
    tempCurve->setPen(QPen(QBrush(QColor(mCurveColors[nCurves])),size));
    tempCurve->setYAxis(axisY);
    mpCurves.append(tempCurve);

    ++nCurves;
    if(nCurves > mCurveColors.size()-1)
    {
        nCurves = 0;        //! @todo Ugly way to restart color loop when too many curves are added
    }

    mpVariablePlot->setAxisTitle(VariablePlot::yLeft, yLabel);
    mpVariablePlot->insertLegend(new QwtLegend(), QwtPlot::TopLegend);
}


void PlotWindow::changeXVector(QVector<double> xarray, QString xLabel)
{
    QVector<double> tempYarray;
    for(size_t i=0; i<mpCurves.size(); ++i)
    {
        for(size_t j=0; j<mpCurves.at(i)->data().size(); ++j)       //! @todo Figure out a less stupid way of replacing only the x values...
        {
            tempYarray.append(mpCurves.at(i)->data().y(j));
        }
        mpCurves.at(i)->setData(xarray, tempYarray);
        tempYarray.clear();
    }
    mpVariablePlot->setAxisTitle(VariablePlot::xBottom, xLabel);
    mHasSpecialXAxis = true;
}


VariablePlot::VariablePlot(QWidget *parent)
        : QwtPlot(parent)
{
    setCanvasBackground(QColor(Qt::white));
    setAutoReplot(true);
}


void VariablePlot::setCurve(QwtPlotCurve *pCurve)
{
    mpCurve = pCurve;
}


QwtPlotCurve *VariablePlot::getCurve()
{
    return mpCurve;
}


VariableList::VariableList(MainWindow *parent)
        : QTreeWidget(parent)
{
    mpParentMainWindow = parent;
    mpCurrentSystem = mpParentMainWindow->mpProjectTabs->getCurrentSystem();

    this->setDragEnabled(true);
    this->setAcceptDrops(false);
    this->updateList();
    this->setHeaderHidden(true);
    this->setColumnCount(1);

    connect(mpParentMainWindow->mpProjectTabs, SIGNAL(currentChanged(int)), this, SLOT(updateList()));
    connect(mpParentMainWindow->mpProjectTabs, SIGNAL(tabCloseRequested(int)), this, SLOT(updateList()));
    connect(mpParentMainWindow->simulateAction, SIGNAL(triggered()), this, SLOT(updateList()));
    connect(this,SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),this,SLOT(createPlot(QTreeWidgetItem*)));
}

void VariableList::updateList()
{
    xMap.clear();
    yMap.clear();
    this->clear();

    if(mpParentMainWindow->mpProjectTabs->count() == 0)     //Check so that at least one project tab exists
    {
        return;
    }

    mpCurrentSystem = mpParentMainWindow->mpProjectTabs->getCurrentTab()->mpSystem;
    QVector<double> y;
    QHash<QString, GUIObject *>::iterator it;
    QTreeWidgetItem *tempComponentItem;
    QTreeWidgetItem *tempParameterItem;
    bool colorize = false;
    for(it = mpCurrentSystem->mGUIObjectMap.begin(); it!=mpCurrentSystem->mGUIObjectMap.end(); ++it)
    {
        QColor backgroundColor;
        if(colorize)
        {
            backgroundColor = QColor("white");
            colorize = false;
        }
        else
        {
            backgroundColor = QColor("white");      //Used to be "beige"
            colorize = true;
        }

        tempComponentItem = new QTreeWidgetItem();
        tempComponentItem->setText(0, it.value()->getName());
        tempComponentItem->setBackgroundColor(0, backgroundColor);
        QFont tempFont;
        tempFont = tempComponentItem->font(0);
        tempFont.setBold(true);
        tempComponentItem->setFont(0, tempFont);
        this->addTopLevelItem(tempComponentItem);

        QList<GUIPort*> portListPtrs = it.value()->getPortListPtrs();
        QList<GUIPort*>::iterator itp;
        for(itp = portListPtrs.begin(); itp !=portListPtrs.end(); ++itp)
        {
           // qDebug() << "Writing plot stuff for " << it.value()->getName() << " " << (*itp)->getName();

            QVector<QString> parameterNames;
            QVector<QString> parameterUnits;
            mpParentMainWindow->mpProjectTabs->getCurrentTab()->mpSystem->mpCoreSystemAccess->getPlotDataNamesAndUnits((*itp)->getGUIComponentName(), (*itp)->getName(), parameterNames, parameterUnits);

            qDebug() << "guiComponentName: " << (*itp)->getGUIComponentName() << " portName: " << (*itp)->getName();
            QVector<double> time = QVector<double>::fromStdVector(mpParentMainWindow->mpProjectTabs->getCurrentTab()->mpSystem->mpCoreSystemAccess->getTimeVector((*itp)->getGUIComponentName(), (*itp)->getName()));
            qDebug() << "time.size: " << time.size();
            if(time.size() > 0)     //If time vector is greater than zero we have something to plot!
            {
                for(int i = 0; i!=parameterNames.size(); ++i)
                {
                    y.clear();
                    tempParameterItem = new QTreeWidgetItem();
                    tempParameterItem->setText(0, (*itp)->getName() + ", " + parameterNames[i] + ", [" + parameterUnits[i] + "]");
                    tempParameterItem->setBackgroundColor(0, backgroundColor);
                    //tempListWidget->setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                    tempComponentItem->addChild(tempParameterItem);
                    mpParentMainWindow->mpProjectTabs->getCurrentTab()->mpSystem->mpCoreSystemAccess->getPlotData((*itp)->getGUIComponentName(), (*itp)->getName(), parameterNames[i], y);
                    xMap.insert((*itp)->getGUIComponentName() + ", " + (*itp)->getName() + ", " + parameterNames[i] + ", [" + parameterUnits[i] + "]", time);
                    yMap.insert((*itp)->getGUIComponentName() + ", " + (*itp)->getName() + ", " + parameterNames[i] + ", [" + parameterUnits[i] + "]", y);
                    yLabelMap.insert((*itp)->getGUIComponentName() + ", " + (*itp)->getName() + ", " + parameterNames[i] + ", [" + parameterUnits[i] + "]", parameterNames[i]);
                }
            }
        }
    }
    this->sortItems(0, Qt::AscendingOrder);
}


//! Creates a new plot window
//! @param *item is the tree widget item whos arrays will be looked up from the map and plotted
void VariableList::createPlot(QTreeWidgetItem *item)
{
    if(item->parent() != 0)     //Top level items cannot be plotted (they represent the components)
    {
        QString lookupName;
        lookupName = QString(item->parent()->text(0) + ", " + item->text(0));

        QString title;
        QString xlabel;
        QString ylabel;

        title.append(lookupName);
        ylabel.append(yLabelMap.find(lookupName).value());
        xlabel.append("Time, [s]");

        PlotWindow *plotWindow = new PlotWindow(xMap.find(lookupName).value(),yMap.find(lookupName).value(), this, mpParentMainWindow);
        plotWindow->setWindowTitle("HOPSAN Plot Window");
        plotWindow->tempCurve->setTitle(title);
        plotWindow->mpVariablePlot->setAxisTitle(VariablePlot::yLeft, ylabel);
        plotWindow->mpVariablePlot->setAxisTitle(VariablePlot::xBottom, xlabel);
        plotWindow->mpVariablePlot->insertLegend(new QwtLegend(), QwtPlot::TopLegend);
        plotWindow->show();
    }
}

void VariableList::mousePressEvent(QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);

    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->pos();
}

void VariableList::mouseMoveEvent(QMouseEvent *event)
{

    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragStartPosition).manhattanLength()
         < QApplication::startDragDistance())
        return;

    QTreeWidgetItem *item = this->currentItem();

    QString mimeText;
    mimeText = QString("HOPSANPLOTDATA " + item->parent()->text(0) + ", " + item->text(0));

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    mimeData->setText(mimeText);
    drag->setMimeData(mimeData);
    drag->exec();
}


SelectedVariableList::SelectedVariableList(MainWindow *parent)
        : VariableList(parent)
{
    mpParentMainWindow = parent;
    mpCurrentSystem = mpParentMainWindow->mpProjectTabs->getCurrentSystem();
    this->setAcceptDrops(true);
    this->setDragEnabled(true);

    xMap.clear();
    yMap.clear();
}

void SelectedVariableList::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << "Dragging something...";
    if (event->mimeData()->hasFormat("application/x-plotvariable"))
    {
        event->acceptProposedAction();
    }
}

//
//void SelectedVariableList::dragMoveEvent(QDragMoveEvent *event)
//{
//    if (event->mimeData()->hasFormat("application/x-plotvariable"))
//    {
//      event->accept();
//    }
//    else
//    {
//        event->ignore();
//    }
//}


void SelectedVariableList::dropEvent(QDropEvent *event)
{
//    qDebug() << "dropEvent";
//    if (event->mimeData()->hasFormat("application/x-plotvariable"))
//    //if (event->mimeData()->hasText())
//    {
//        qDebug() << "True!";
//        QString datastr =  event->mimeData()->text();
//        //QTextStream stream(&datastr, QIODevice::ReadOnly);
//        QListWidgetItem *tempListWidget;
//        tempListWidget = new QListWidgetItem(datastr, this);

//        event->acceptProposedAction();
//   }
}

VariableListDialog::VariableListDialog(MainWindow *parent)
        : QWidget(parent)
{
    //this->setAcceptDrops(true);

    mpParentMainWindow = parent;

    //Create a grid
    QGridLayout *grid = new QGridLayout(this);

    //Create the plotvariables list
    mpVariableList = new VariableList(mpParentMainWindow);
    SelectedVariableList *rightAxisList = new SelectedVariableList(mpParentMainWindow);
    SelectedVariableList *leftAxisList = new SelectedVariableList(mpParentMainWindow);
    rightAxisList->setMaximumHeight(100);
    rightAxisList->setObjectName("Right Axis");
    rightAxisList->setWindowTitle("Right Axis");
    rightAxisList->setSelectionMode(QAbstractItemView::SingleSelection);
    rightAxisList->setDragEnabled(true);
    rightAxisList->viewport()->setAcceptDrops(true);
    rightAxisList->setDropIndicatorShown(true);
    rightAxisList->setDragDropMode(QAbstractItemView::DragDrop);
    leftAxisList->setMaximumHeight(100);
    leftAxisList->setWindowTitle("Left Axis");

    plotButton = new QPushButton(tr("&Plot"));
    plotButton->setAutoDefault(true);

    grid->addWidget(mpVariableList,0,0,3,1);
    //grid->addWidget(rightAxisList,4,0,1,1);
    //grid->addWidget(leftAxisList,5,0,1,1);
    //grid->addWidget(plotButton,6,0);
}
