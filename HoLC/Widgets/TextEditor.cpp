#include <QPainter>
#include <QPaintEvent>
#include <QTextBlock>
#include <QTextDocumentFragment>
#include <QDebug>
#include <math.h>

#include "TextEditor.h"


//! @brief Constructor for text editor that handles line numbers and replaces tabs with spaces
TextEditor::TextEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    mpLineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}


//! @brief Computes and returns the width of the line number area
int TextEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}


//! @brief Updates width of line number area
void TextEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


//! @brief Update function for line number area
void TextEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        mpLineNumberArea->scroll(0, dy);
    else
        mpLineNumberArea->update(0, rect.y(), mpLineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}


//! @brief Resize event for text editor
void TextEditor::resizeEvent(QResizeEvent *pEvent)
{
    QPlainTextEdit::resizeEvent(pEvent);

    QRect cr = contentsRect();
    mpLineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}


//! @brief Intercepts paste operations, to control what is pastable
void TextEditor::insertFromMimeData(const QMimeData *pData)
{
    if(pData->hasText())
    {
        insertPlainText(pData->text());
    }
}


//! @brief Intercept key press event, to handle special keys
void TextEditor::keyPressEvent(QKeyEvent *pEvent)
{
    //Replace tabs with four whitespaces
    if(pEvent->key() == Qt::Key_Backtab)    //Shift+tab, remove spaces to the left until next tab stop (or as many spaces as possible)
    {
        QString line = this->textCursor().block().text().toAscii();
        int pos = textCursor().positionInBlock();
        while(pos > 0 && line.at(pos-1) == ' ')
        {
            this->textCursor().deletePreviousChar();
            line = this->textCursor().block().text().toAscii();
            pos = textCursor().positionInBlock();
            if(pos%4 == 0)
            {
                break;
            }
        }
    }
    else if(pEvent->key() == Qt::Key_Tab)
    {
        if(textCursor().anchor() != textCursor().position())    //Selection exists, indent whole selection
        {
            QString text = textCursor().selection().toPlainText();
            text.replace("\n", "\n    ");
            text.prepend("    ");
            textCursor().beginEditBlock();
            textCursor().removeSelectedText();
            insertPlainText(text);
            textCursor().endEditBlock();
        }
        else
        {
            QString line = this->textCursor().block().text().toAscii();
            int pos = textCursor().positionInBlock();
            qDebug() << line << ", position: " << pos;
            int nSpaces = 0;
            while(line.startsWith(" "))
            {
                nSpaces++;
                line.remove(0,1);
            }
            int nSpacesToInsert = 4;    //Inssert four spaces by default
            if(pos < nSpaces)           //If at beginning of line, insert until next tab stop
            {
                while((nSpaces+nSpacesToInsert)%4 != 0)
                {
                    nSpacesToInsert--;
                }
            }
            for(int i=0; i<nSpacesToInsert; ++i)
            {
                this->insertPlainText(" ");
            }
        }
    }
    else
    {
        QPlainTextEdit::keyPressEvent(pEvent);
    }
}


//! @brief Highlights current line
void TextEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}


//! @brief Paint event for line number area
void TextEditor::lineNumberAreaPaintEvent(QPaintEvent *pEvent)
{
    QPainter painter(mpLineNumberArea);
    painter.fillRect(pEvent->rect(), QColor(240,240,240)/*Qt::lightGray*/);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = int(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + int(blockBoundingRect(block).height());

    while (block.isValid() && top <= pEvent->rect().bottom())
    {
        if (block.isVisible() && bottom >= pEvent->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, mpLineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}


//void TextEditor::updateLineNumberArea(const QRect &rect, int dy)
//{
//    if (dy)
//    {
//        mpLineNumberArea->scroll(0, dy);
//    } else if (m_countCache.first != blockCount() || m_countCache.second != textCursor().block().lineCount())
//    {
//        mpLineNumberArea->update(0, rect.y(), mpLineNumberArea->width(), rect.height());
//        m_countCache.first = blockCount();
//        m_countCache.second = textCursor().block().lineCount();
//    }

//    if (rect.contains(viewport()->rect()))
//        updateLineNumberAreaWidth(0);
//}
