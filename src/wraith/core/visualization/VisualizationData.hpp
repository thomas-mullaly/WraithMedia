/*
 * Copyright 2010,2011 Thomas Mullaly
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */
#ifndef WRAITH_VISUALIZATION_VISUALIZATION_DATA_HPP
#define WRAITH_VISUALIZATION_VISUALIZATION_DATA_HPP

#include <vector>

#include <boost/shared_ptr.hpp>

namespace wraith { namespace visualization {

class VisualizationData {
public:
    typedef std::vector<float>                      ChannelData;
    typedef std::vector<float>                      SpectrumData;
    typedef std::vector<ChannelData>                PCMData;
    typedef boost::shared_ptr<VisualizationData>    VisualizationDataPtr;

    VisualizationData(std::size_t channels, std::size_t samples, float *pcm,
                      std::size_t bands, float *spectrum);
    virtual ~VisualizationData();

    virtual std::size_t channels() const;
    virtual std::size_t samples() const;
    virtual PCMData const& pcm() const;
    virtual std::size_t bands() const;
    virtual SpectrumData const& spectrum() const;

private:
    void loadPCMData(float *pcm, std::size_t samples, std::size_t channels);
    void loadSpectrumData(float *spectrum, std::size_t bands);

protected:
    std::size_t     m_channels;
    std::size_t     m_samples;
    PCMData         m_pcm;
    std::size_t     m_bands;
    SpectrumData    m_spectrum;
};

} }

#endif
