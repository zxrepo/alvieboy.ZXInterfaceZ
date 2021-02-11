#include "filefilter.h"
#include "fileaccess.h"
#include "log.h"

#define TAG "FileFilter"

bool FileFilter::match_extension(const char *ext) const
{
    unsigned i;

    if (m_count==0) {
        ESP_LOGI(TAG, "Always match entries (%s)", getDescription());
        return true;
    }
    if (m_count<0) {
        ESP_LOGI(TAG, "Never match entries");
        return false;
    }

    for (i=0; i<m_count;i++) {
        ESP_LOGI(TAG, "Matching %s %s", ext, m_ext[i]);
        if (ext_match(ext, m_ext[i]))
            return true;
    }
    return false;
}