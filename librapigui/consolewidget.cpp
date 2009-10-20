/***************************************************************************
 * Project: RAPI                                                           *
 * Author:  Jens Wawerla (jwawerla@sfu.ca)                                 *
 * $Id: $
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 **************************************************************************/
#include "consolewidget.h"
#include "assert.h"
namespace Rapi
{

/** Maximal number of characters in the text edit field */
const int MAX_CHARACTERS = 10000;

//-----------------------------------------------------------------------------
CConsoleWidget::CConsoleWidget ( std::string* str, QWidget* parent )
    : ADeviceWidget ( parent )
{
  QFont font("Courier", 10);
  setTitle ( "Console" );
  mGeneralInfoBox->setHidden ( true );

  assert ( str );
  mRPrintfString = str;

  mPlainTextEdit = new QPlainTextEdit ( this );
  mMainLayout->addWidget ( mPlainTextEdit );
  mPlainTextEdit->setReadOnly ( true );
  font.setStyleHint(QFont::TypeWriter);
  mPlainTextEdit->setFont(font);
  // limit text output to 1000 lines
  mPlainTextEdit->setMaximumBlockCount(1000);
}
//-----------------------------------------------------------------------------
CConsoleWidget::~CConsoleWidget()
{
}
//-----------------------------------------------------------------------------
void CConsoleWidget::updateData()
{
  QString text;
  if ( isChecked() ) {
    mPlainTextEdit->setHidden ( false );
    if (mRPrintfString->empty() == false) {
      text = mRPrintfString->c_str();
      // if the text ends with a return, we need to remove it, because
      // appendPlainText will add one
      if (text.endsWith('\n') ) {
        text = text.remove(text.length()-1, 1);
      }
      mPlainTextEdit->appendPlainText ( text );
      mRPrintfString->clear();
    }
  }
  else {
    mPlainTextEdit->setHidden ( true );
  }
  mPlainTextEdit->update();
}
//-----------------------------------------------------------------------------

} // namespace
