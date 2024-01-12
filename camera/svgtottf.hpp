#ifndef SVGTOttf_HPP_
#define SVGTOttf_HPP_

#include <iostream>
#include <QString>
#include <QStringList>
class SVGtoTTF {
public:
    void createTtfFromSvg(const QString &outputFileName, const QStringList &svgFileList);
    int convert(const std::string& characters_dir, const std::string& output_directory, const std::string& config, const std::string& metadata, const std::string& stylename);
};

#endif // SVGTOttf_HPP_
