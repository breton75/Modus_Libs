#include "proj_12700_opa_mod.h"

opa::SvOPA::SvOPA():
  modus::SvAbstractProtocol()
{
  signal_collections.insert(TYPE_0x33, &type0x33_signals);
  signal_collections.insert(TYPE_0x02, &type0x02_signals);
  signal_collections.insert(TYPE_0x03, &type0x03_signals);
  signal_collections.insert(TYPE_0x04, &type0x04_signals);
  signal_collections.insert(TYPE_0x19, &type0x19_signals);
}

bool opa::SvOPA::configure(const modus::DeviceConfig &cfg)
{
  try {

    p_config = cfg;

    m_dev_params = opa::DeviceParams::fromJson(p_config.dev_params);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

void opa::SvOPA::disposeSignal (modus::SvSignal* signal)
{
  try {

    QString tag = signal->config()->type.toLower();

    if(tag == "status")
      line_status_signals.addSignal(signal);

    else {

      bool ok;
      int itag = tag.toInt(&ok, 0);

      if(ok && signal_collections.contains(itag))
        signal_collections.value(itag)->addSignal(signal);

      else {

        emit message(QString("Сигнал %1: Неопознанный тип \"%2\"").arg(signal->config()->name).arg(signal->config()->type),
                     sv::log::llError, sv::log::mtError);

      }
    }
  }

  catch(SvException e) {
    throw e;
  }
}

bool opa::SvOPA::parse_input_data()
{
  bool parsed = false;

  if(p_input_buffer->offset >= m_hsz) {

    memcpy(&m_header, &p_input_buffer->buf[0], m_hsz);

    if((m_header.client_addr != 1) || (m_header.func_code != 0x10)) {

      return parsed;
    }

    if(p_input_buffer->offset >= m_hsz + m_header.byte_count + 2) {

        quint16 current_register = (static_cast<quint16>(m_header.ADDRESS << 8)) + m_header.OFFSET;

        if((current_register < m_dev_params.start_register) ||
           (current_register > m_dev_params.last_register))
        {
           return parsed;
        }

qDebug() << 333;
        emit message(QString(QByteArray((const char*)&p_input_buffer->buf[0], p_input_buffer->offset).toHex()));

        // если хоть какие то пакеты сыпятся (для данного получателя), то
        // считаем, что линия передачи в порядке и задаем новую контрольную точку времени
//        p_device->setNewLostEpoch();

        // ставим состояние данной линии
        line_status_signals.updateSignals();

        // парсим и проверяем crc
        memcpy(&m_data.data_type,   &p_input_buffer->buf[0] + m_hsz, 1);                       // тип данных
        memcpy(&m_data.data_length, &p_input_buffer->buf[0] + m_hsz + 1, 1);                   // длина данных
        memcpy(&m_data.data[0],     &p_input_buffer->buf[0] + m_hsz + 2, m_data.data_length);  // данные
        memcpy(&m_data.crc,         &p_input_buffer->buf[0] + m_hsz + m_header.byte_count, 2); // crc полученная

        quint16 calc_crc = CRC::MODBUS_CRC16((const quint8*)&p_input_buffer->buf[0], m_hsz + m_header.byte_count); // вычисляем crc из данных

        if(calc_crc != m_data.crc) {

          // если crc не совпадает, то выходим без обработки и ответа
          emit message(QString("Ошибка crc! Ожидалось %1, получено %2").arg(calc_crc, 0, 16).arg(m_data.crc, 0, 16),
                       sv::log::llError, sv::log::mtError);

          return parsed;

        }
        else {

           switch (current_register - m_dev_params.start_register)
           {
               case 0x00:
               case 0x03:
               case 0x05:

                 // здесь просто отправляем ответ-квитирование
                 confirmation();

                 if(m_data.data_type == 0x77) {
                   foreach (modus::SvSignal* signal, p_input_signals.values())
                     signal->setValue(0);
                 }

                 parsed = true;

                 break;

               case 0x06:
               case 0x10:
               case 0x50:
               case 0x90:
               {
                 // парсим и проверяем crc
   //              quint16 calc_crc = parse_data(&p_buff, &m_data, &m_header);

                  // формируем и отправляем ответ-квитирование
                  confirmation();

                  if(signal_collections.contains(m_data.data_type))
                    signal_collections.value(m_data.data_type)->updateSignals(&m_data);

                  parsed = true;

                  break;
               }

               default:
                   break;
           }
        }
    }
  }

  return parsed;

}

bool opa::SvOPA::form_output_data()
{
  return true;
}

void opa::SvOPA::confirmation()
{
  QByteArray confirm;
  confirm.append((const char*)(&m_header), 6);

  // вычисляем crc ответа
  quint16 crc = CRC::MODBUS_CRC16((uchar*)(&m_header), 6);
  confirm.append(quint8(crc & 0xFF));
  confirm.append(quint8(crc >> 8));

  memcpy(p_output_buffer, confirm.data(), confirm.length());
  p_output_buffer->offset = confirm.length();

//  emit outputBufferReady();

}


/** ********** EXPORT ************ **/
modus::SvAbstractProtocol* create()
{
  modus::SvAbstractProtocol* device = new opa::SvOPA();
  return device;
}
