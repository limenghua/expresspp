#ifndef EXPRESSPP_EXPRESSPP_HPP
#define EXPRESSPP_EXPRESSPP_HPP

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <expresspp/router.hpp>
#include <list>

#include <unordered_map>
#include <boost/any.hpp>


namespace expresspp
{
	class server{
	public:
		typedef websocketpp::server<websocketpp::config::asio> impl_type;
		server()
		{
		}


		server & listen(unsigned short port){

			m_impl.init_asio();

			m_impl.set_open_handler(bind(&server::on_open, this, std::placeholders::_1));
			m_impl.set_close_handler(bind(&server::on_close, this, std::placeholders::_1));
			m_impl.set_message_handler(bind(&server::on_message, this, std::placeholders::_1, std::placeholders::_2));
			m_impl.set_http_handler(bind(&server::on_http, this, std::placeholders::_1));

			//use(bind(&router::dispatch, &m_router, std::placeholders::_1, std::placeholders::_2));

			m_impl.listen(port);
			m_impl.start_accept();
			return *this;
		}

		server & run(){
			m_impl.run();
			return *this;
		}

		server & route(const std::string & path, handle_function handler)
		{
			m_router.route(path, handler);
			return *this;
		}

		server & use(handle_function handler)
		{
			m_handlers.push_back(handler);
			return *this;
		}

		server & before(handle_function handler)
		{
			m_before.push_back(handler);
			return *this;
		}

		server & after(handle_function handler)
		{
			m_after.push_back(handler);
			return *this;
		}


		template<class T>
		void set(const std::string &key,const T & value)
		{
			m_settings.emplace(key, value);
		}


		template<class T>
		T& get(const std::string &key)
		{
			auto it = m_settings.find(key);
			if (it == m_settings.end())
			{
				throw std::runtime_error("No such key.");
			}

			return boost::any_cast<T&>(it->second);
		}

	protected:
		impl_type & websocketpp()
		{
			return m_impl;
		}


	private:
		void on_open(websocketpp::connection_hdl hdl) {

		}

		void on_close(websocketpp::connection_hdl hdl) {

		}

		void on_message(websocketpp::connection_hdl hdl, impl_type::message_ptr msg) {

		}
		/*
		 * 收到http请求依照如下次序处理：
		 * 1. 调用所有before 处理器
		 * 2. 调用用户通过use注册处理器
		 * 3. 调用router.dipatch分发请求
		 * 4. 调用所有after 处理器
		*/
		void on_http(websocketpp::connection_hdl hdl)
		{
			impl_type::connection_ptr con = m_impl.get_con_from_hdl(hdl);

			request	req = con->get_request();
			response res;

			for (auto fun : m_before){
				fun(req, res);
			}

			for (auto fun : m_handlers){
				fun(req, res);
				if (res.is_done()){
					break;
				}
			}

			if (!res.is_done()){
				m_router.dispatch(req, res);
			}

			for (auto fun : m_after){
				fun(req, res);
			}

			if (!res.is_done()){
				res.end();
			}

			con->set_body(res.get_body());
			con->set_status(websocketpp::http::status_code::ok);
		}


	private:
		impl_type m_impl;
		router m_router;

		std::list<handle_function> m_before;
		std::list<handle_function> m_after;
		std::list<handle_function> m_handlers;

		std::unordered_map<std::string, boost::any> m_settings;
	};

}




#endif //EXPRESSPP_EXPRESSPP_HPP