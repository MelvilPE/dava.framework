#ifndef _PATHMANIP_H
#define _PATHMANIP_H

#include "Base/BaseTypes.h"

namespace DAVA
{
// TODO
class PathManip
{
public:
    PathManip(const char* src);
    //PathManip(const PathManip& orig);
    //virtual ~PathManip();
    /**
		 * Returns path as string
		 */
    String GetString();

    /// returns path to last entry
    String GetPath();

    /// returns last entry with suffix
    String GetName();

    /// returns suffix, e.g. ".png" or empty string if none
    String getSuffix();

    /**
     * @brief Sets the suffix string
     * @param[in] s String to be set as suffix
     */
    void setSuffix(const String& s);

private:
    List<String> pathEntries;

    /**
     * @brief Splits the provided string path into individual entries/components
     * 
     * @param src Input string containing the path to be split
     * 
     * @details This function takes a string path and splits it into separate components,
     *          storing the resulting entries internally. The splitting is typically performed
     *          using path separators (like '/' or '\') as delimiters.
     */
    void splitToEntries(const char* src);
};

} // ns

#endif /* _PATHMANIP_H */
