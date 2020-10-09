#include "sv_opa.h"


SvOPA::SvOPA(sv::SvAbstractLogger *logger)
{
}




void opa::UDPThread::run()
{
  p_is_active = true;

  while(p_is_active) {

    process_signals();

    while(m_socket.waitForReadyRead(1000) && p_is_active) {

      while(m_socket.hasPendingDatagrams() && p_is_active)
      {
        if(m_socket.pendingDatagramSize() <= 0)
          continue;

        if(p_buff.offset > MAX_PACKET_SIZE)
          reset_buffer();

        if(!p_is_active)
          break;

        /* ... the rest of the datagram will be lost ... */
        p_buff.offset += m_socket.readDatagram((char*)(&p_buff.buf[p_buff.offset]), MAX_PACKET_SIZE - p_buff.offset);

        process_data();

      }
    }
  }

  m_socket.close();

}




void opa::SerialThread::run()
{
  p_is_active = true;

  while(p_is_active) {

    while(p_port.waitForReadyRead(1)) {
      if(p_buff.offset > MAX_PACKET_SIZE)
        reset_buffer();

      p_buff.offset += p_port.read((char*)(&p_buff.buf[p_buff.offset]), MAX_PACKET_SIZE - p_buff.offset);

      process_data();

    }
  }

  p_port.close();

}
