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
  QFont font("Currier", 10);
  setTitle ( "Console" );
  mGeneralInfoBox->setHidden ( true );

  assert ( str );
  mRPrintfString = str;

  mTextEdit = new QTextEdit ( this );
  mMainLayout->addWidget ( mTextEdit );
  mTextEdit->setReadOnly ( true );
  font.setStyleHint(QFont::TypeWriter);
  mTextEdit->setCurrentFont(font);
}
//-----------------------------------------------------------------------------
CConsoleWidget::~CConsoleWidget()
{
}
//-----------------------------------------------------------------------------
void CConsoleWidget::updateData()
{
  int len;
  QString text;
  if ( isChecked() ) {
    mTextEdit->setHidden ( false );

    // make sure the text doen't grow too big
    len = mTextEdit->toPlainText().length();
    if (len > MAX_CHARACTERS) {
      text = mTextEdit->toPlainText();
      text = text.remove(0, len - MAX_CHARACTERS);
      mTextEdit->clear();
      mTextEdit->append(text);
    }
    mTextEdit->append ( mRPrintfString->c_str() );
    mRPrintfString->clear();
  }
  else {
    mTextEdit->setHidden ( true );
  }
  mTextEdit->update();
}
//-----------------------------------------------------------------------------

} // namespace
