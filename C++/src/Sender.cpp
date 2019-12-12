#include "Sender.hpp"

Sender::Sender(std::string address, std::string client_id , int qos, std::string persist_dir)
{
	m_address = address;
	m_qos = qos;
	m_persist_dir = persist_dir;
	m_client_id = client_id;

	m_cli = new mqtt::async_client(m_address, client_id, MAX_BUFFERED_MSGS, m_persist_dir);

	m_conn_opt.set_keep_alive_interval(MAX_BUFFERED_MSGS);
	m_conn_opt.set_clean_session(true);
	m_conn_opt.set_automatic_reconnect(true);

	try {
		// Connect to the MQTT broker
		cout << "MQTT version: " << m_conn_opt.get_mqtt_version() << endl;
		cout << "Connecting to server '" << address << "'..." << flush;
		m_cli->connect(m_conn_opt)->wait();
		cout << "OK\n" << endl;
	}
	catch (const mqtt::exception& exc) {
		cerr << exc.what() << endl;
	}
}

Sender::~Sender()
{
	if(m_cli != nullptr)
	{
		cout << "\nDisconnecting..." << flush;
		m_cli->disconnect()->wait();
		cout << "OK" << endl;
		delete m_cli;
	}
}

void Sender::send(std::string payload, std::string topic)
{
	mqtt::message_ptr msg = mqtt::make_message(topic, payload);
	msg->set_qos(m_qos);
	m_cli->publish(msg);
}
