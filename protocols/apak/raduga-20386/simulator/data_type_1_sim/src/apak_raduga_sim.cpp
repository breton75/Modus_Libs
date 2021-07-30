#include "apak_raduga_sim.h"

raduga::SvRaduga::SvRaduga():
  modus::SvAbstractProtocol()
{
  output_signal_collections.insert(TYPE_1,  &type1_output_signals);
  output_signal_collections.insert(TYPE_2,  &type2_output_signals);
  output_signal_collections.insert(TYPE_3,  &type3_output_signals);
  output_signal_collections.insert(TYPE_5,  &type5_output_signals);
  output_signal_collections.insert(TYPE_9,  &type9_output_signals);
  output_signal_collections.insert(TYPE_53, &type53_output_signals);
}

bool raduga::SvRaduga::configure(modus::DeviceConfig *config, modus::IOBuffer *iobuffer)
{
  try {

    p_config = config;
    p_io_buffer = iobuffer;

    m_params = raduga::ProtocolParams::fromJson(p_config->protocol.params);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

void raduga::SvRaduga::disposeInputSignal (modus::SvSignal* signal)
{
  Q_UNUSED(signal);
}

void raduga::SvRaduga::disposeOutputSignal (modus::SvSignal* signal)
{
  try {

    bool ok;
    quint16 itype = signal->config()->type.toUInt(&ok, 0);

    if(ok && output_signal_collections.contains(itype))
      output_signal_collections.value(itype)->addSignal(signal, p_config->bufsize);

    else {

      emit message(QString("Сигнал %1: Неопознанный тип данных \"%2\"").arg(signal->config()->name).arg(signal->config()->tag),
                   sv::log::llError, sv::log::mtError);

    }
  }

  catch(SvException& e) {
    throw e;
  }
}

void raduga::SvRaduga::run()
{
  p_is_active = bool(p_config) && bool(p_io_buffer);

  while(p_is_active) {

    p_io_buffer->output->mutex.lock();
    putout();
    p_io_buffer->output->mutex.unlock();

    msleep(m_params.interval);

  }
}

void raduga::SvRaduga::putout()
{
//  qDebug() << "putout" << m_params.packid;
  p_io_buffer->output->offset = 0;

  memcpy(&p_io_buffer->output->data[p_io_buffer->output->offset], &RDGA_NAME[0], SYSTEM_NAME_LEN);
  p_io_buffer->output->offset += SYSTEM_NAME_LEN;

  memcpy(&p_io_buffer->output->data[p_io_buffer->output->offset], &m_params.abonent, sizeof(quint16));
  p_io_buffer->output->offset += sizeof(quint16);

  memcpy(&p_io_buffer->output->data[p_io_buffer->output->offset], &m_params.activity, sizeof(quint16));
  p_io_buffer->output->offset += sizeof(quint16);

  memcpy(&p_io_buffer->output->data[p_io_buffer->output->offset], &m_params.packid, sizeof(quint16));
  p_io_buffer->output->offset += sizeof(quint16);

  switch (m_params.packid) {

    case 101:
    case 105:
    case 106:
    case 110: {

      output_signal_collections.value(TYPE_1)->updateOutput(p_io_buffer->output);
//      qDebug() << "putout" << p_io_buffer->output->offset << int(p_io_buffer->output->data[256]);
      p_io_buffer->output->offset += 1152;
      break;
    }

    case 102:
    case 107:

      output_signal_collections.value(TYPE_2)->updateOutput(p_io_buffer->output);
      p_io_buffer->output->offset += 384;
      output_signal_collections.value(TYPE_3)->updateOutput(p_io_buffer->output);
      p_io_buffer->output->offset += 384;

      break;

    case 103:
    case 108:

      output_signal_collections.value(TYPE_5)->updateOutput(p_io_buffer->output);
      p_io_buffer->output->offset += 1284;

      break;

    case 104:
    case 109:

      output_signal_collections.value(TYPE_9)-> updateOutput(p_io_buffer->output);
      p_io_buffer->output->offset += 316;
      output_signal_collections.value(TYPE_53)->updateOutput(p_io_buffer->output);
      p_io_buffer->output->offset += 384;

      break;

  }

  quint16 crc = CRC::MODBUS_CRC16((quint8*)&p_io_buffer->output->data[m_hsz], p_io_buffer->output->offset - m_hsz);
  memcpy(&p_io_buffer->output->data[p_io_buffer->output->offset], &crc, sizeof(qint16));
  p_io_buffer->output->offset += 2;

}

/** ********** EXPORT ************ **/
modus::SvAbstractProtocol* create()
{
  modus::SvAbstractProtocol* device = new raduga::SvRaduga();
  return device;
}
