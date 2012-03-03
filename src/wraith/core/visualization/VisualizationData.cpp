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
#include "VisualizationData.hpp"

namespace wraith { namespace visualization {

VisualizationData::VisualizationData(std::size_t channels, std::size_t samples, float *pcm,
                                     std::size_t bands, float *spectrum)
    : m_channels(channels)
    , m_samples(samples)
    , m_pcm()
    , m_bands(bands)
    , m_spectrum()
{
    loadSpectrumData(spectrum, bands);
    loadPCMData(pcm, samples, channels);
}

VisualizationData::~VisualizationData()
{
    for(std::size_t i = 0; i < m_pcm.size(); ++i)
        m_pcm[i].clear();
    m_pcm.clear();
    m_spectrum.clear();
}

std::size_t VisualizationData::channels() const {
    return m_channels;
}

std::size_t VisualizationData::samples() const {
    return m_samples;
}

VisualizationData::PCMData const&
VisualizationData::pcm() const {
    return m_pcm;
}

std::size_t VisualizationData::bands() const {
    return m_bands;
}

VisualizationData::SpectrumData const&
VisualizationData::spectrum() const {
    return m_spectrum;
}

void VisualizationData::loadPCMData(float *raw_pcm, std::size_t samples, std::size_t channels) {
    for(std::size_t channel = 0; channel < channels; ++channel) {
        ChannelData channel_data;

        for(std::size_t sample = 0; sample < samples; ++sample) {
            channel_data.push_back(raw_pcm[channel * samples + sample]);
        }

        m_pcm.push_back(channel_data);
    }
}

void VisualizationData::loadSpectrumData(float *spectrum, std::size_t bands) {
    for(std::size_t band = 0; band < bands; ++band) {
        m_spectrum.push_back(spectrum[band]);
    }
}

} }
