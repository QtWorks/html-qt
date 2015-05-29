#include <QCoreApplication>
#include <QCommandLineParser>

#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>

#include <QTextStream>
#include <QFile>

#include "../html-qt/htmltokenizer.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Cutelyst");
    QCoreApplication::setOrganizationDomain("cutelyst.org");
    QCoreApplication::setApplicationName("html-qt");
    QCoreApplication::setApplicationVersion("0.0.1");

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    QCoreApplication::installTranslator(&qtTranslator);

    QCommandLineParser parser;
    parser.setApplicationDescription("Parses HTML documents according to WHATWG definitions");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source HTML file to parse."));

    // Process the actual command line arguments given by the user
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    QTextStream *in = 0;
    if (args.isEmpty()) {
        in = new QTextStream(stdin);
    } else if (args.size() == 1) {
        QFile *file = new QFile(args.first());
        if (!file->open(QFile::ReadOnly)) {
            qFatal("Failed to open html file");
        }
        in = new QTextStream(file);
    } else {
        parser.showHelp(1);
    }

    HTMLTokenizer tokenizer;
    tokenizer.setTextStream(in);
    tokenizer.start();

    return 0;
}