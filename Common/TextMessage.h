#ifndef TEXTMESSAGE_H
#define TEXTMESSAGE_H

#include <QCoreApplication>
#include <QString>
#include <QtGlobal>

namespace message_text {

inline QString aboutProgram()
{
    return QStringLiteral(
               "<p><b>%1 %2</b></p>"
               "<p>Based on Qt %3</p>"
               "<p>Copyright (C) 2023-2026 TheKramarch, EvtoButcher and contributors.</p>"
               "<p>The automated logistics information system is a desktop application "
               "for creating orders for the transportation of goods between warehouses, "
               "displaying routes on a map and simulating truck movement.</p>"
               "<p>This program is distributed under the MIT License and is provided "
               "\"AS IS\", without warranty of any kind.</p>")
        .arg(QCoreApplication::applicationName(),
             QCoreApplication::applicationVersion(),
             QString::fromLatin1(qVersion()));
}

} // namespace message_text

#endif // TEXTMESSAGE_H
