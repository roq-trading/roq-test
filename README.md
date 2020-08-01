# roq-test

Test harness for order management.


## Operating Systems

* Linux


## Library/Package Dependencies

* [fmt](https://github.com/fmtlib/fmt) (MIT License)
* [gflags](https://github.com/gflags/gflags) (BDS 3-Clause License)
* [roq-api](https://github.com/roq-trading/roq-api) (MIT License)
* roq-client (Commerical License)
* [roq-logging](https://github.com/roq-trading/roq-api) (MIT License)

Optional

* [Google Test](https://github.com/google/googletest) (BSD 3-Clause License)


## Prerequisites

The project is designed to be compatible with the conda package manager.

```bash
wget -N https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh

bash Miniconda3-latest-Linux-x86_64.sh -b -u -p ~/miniconda3

source ~/miniconda3/bin/activate

conda install -y \
    git \
    cmake \
    gxx_linux-64

conda install -y --channel https://roq-trading.com/conda/stable \
    roq-client
```


## Building

```bash
git submodule update --init --recursive

cmake .

make -j4
```


## Using

The command-line interface can be used like this

```bash
roq-test \
    --name "test" \
    --exchange "deribit" \
    --symbol "BTC-PERPETUAL" \
    --enable-modify-order \
    ~/deribit.sock
```

Note! It is assumed that a market gateway is listening on `~/deribit.sock` (a
UNIX domain socket end-point).

This will attempt to do the following

* Place a buy order 5 ticks from best bid
* Wait 30 seconds
* Modify order to best bid
* Wait 30 seconds
* Cancel order

A number of situations could cause the test to terminate early, for example

* Disconnect between gateway and market
* Order status is complete (filled)
* Order status is cancelled (perhaps from a user interface)

Typical output would look something like this

```text
I0723 20:39:43.809311 17135 application.cpp:42] ===== START =====
I0723 20:39:43.809609 17135 service.cpp:39] The metrics service will *not* be started
I0723 20:39:43.812137 17135 controller.cpp:108] Dispatching...
I0723 20:39:43.812173 17135 controller.cpp:112] Starting event loop thread...
I0723 20:39:43.812390 17136 controller.cpp:148] Event loop thread is now running
I0723 20:39:44.812470 17136 session_manager.cpp:44] Connecting "unix:///home/thraneh/deribit.sock"
I0723 20:39:44.818315 17136 session.cpp:38] Adding name="deribit" (user_id=5)
I0723 20:39:44.818374 17135 pollster.cpp:403] Adding name="deribit" (user_id=5)
I0723 20:39:44.818398 17135 strategy.cpp:132] Connected
I0723 20:39:44.818497 17135 strategy.cpp:140] Downloading market data ...
I0723 20:39:44.818504 17135 strategy.cpp:169] Market data is READY
I0723 20:39:44.818858 17135 strategy.cpp:150] download_end={account="", max_order_id=0}
I0723 20:39:44.818861 17135 strategy.cpp:154] Download market data has COMPLETED
I0723 20:39:44.818868 17135 strategy.cpp:143] Downloading account data ...
I0723 20:39:44.818875 17135 strategy.cpp:182] Order manager is READY
I0723 20:39:44.818881 17135 strategy.cpp:150] download_end={account="A1", max_order_id=1000}
I0723 20:39:44.818884 17135 strategy.cpp:157] Download account data has COMPLETED
I0723 20:39:44.818888 17135 strategy.cpp:274] *** INSTRUMENT READY ***
I0723 20:39:46.973212 17135 strategy.cpp:261] *** READY TO TRADE ***
I0723 20:39:46.973502 17135 strategy.cpp:56] create_order={account="A1", order_id=1001, exchange="deribit", symbol="BTC-PERPETUAL", side=BUY, quantity=1.0, order_type=LIMIT, price=9606.5, time_in_force=GTC, position_effect=UNDEFINED, execution_instruction=UNDEFINED, stop_price=nan, max_show_quantity=nan, order_template=""}
I0723 20:39:46.973586 17135 strategy.cpp:225] order_ack={account="A1", order_id=1001, type=CREATE_ORDER, origin=GATEWAY, status=FORWARDED, error=UNDEFINED, text="", gateway_order_id=10000001, external_order_id="", request_id="roq-1595529571-15"}
I0723 20:39:46.996260 17135 strategy.cpp:225] order_ack={account="A1", order_id=1001, type=CREATE_ORDER, origin=EXCHANGE, status=ACCEPTED, error=UNDEFINED, text="success", gateway_order_id=10000001, external_order_id="4237332692", request_id="roq-1595529571-15"}
I0723 20:39:46.996281 17135 strategy.cpp:233] order_update={account="A1", order_id=1001, exchange="deribit", symbol="BTC-PERPETUAL", status=WORKING, side=BUY, price=9606.5, remaining_quantity=1.0, traded_quantity=0.0, position_effect=UNDEFINED, order_template="", create_time_utc=1595529586984000000ns, update_time_utc=1595529586984000000ns, gateway_order_id=10000001, external_order_id="4237332692"}
I0723 20:40:16.997113 17135 strategy.cpp:76] modify_order={account="A1", order_id=1001, quantity=1.0, price=9608.0}
I0723 20:40:16.997227 17135 strategy.cpp:225] order_ack={account="A1", order_id=1001, type=MODIFY_ORDER, origin=GATEWAY, status=FORWARDED, error=UNDEFINED, text="", gateway_order_id=10000001, external_order_id="4237332692", request_id="roq-1595529571-16"}
I0723 20:40:17.026492 17135 strategy.cpp:225] order_ack={account="A1", order_id=1001, type=MODIFY_ORDER, origin=EXCHANGE, status=ACCEPTED, error=UNDEFINED, text="success", gateway_order_id=10000001, external_order_id="4237332692", request_id="roq-1595529571-16"}
I0723 20:40:17.026515 17135 strategy.cpp:233] order_update={account="A1", order_id=1001, exchange="deribit", symbol="BTC-PERPETUAL", status=WORKING, side=BUY, price=9608.0, remaining_quantity=1.0, traded_quantity=0.0, position_effect=UNDEFINED, order_template="", create_time_utc=1595529586984000000ns, update_time_utc=1595529617014000000ns, gateway_order_id=10000001, external_order_id="4237332692"}
I0723 20:40:47.027730 17135 strategy.cpp:89] cancel_order={account="A1", order_id=1001}
I0723 20:40:47.027839 17135 strategy.cpp:225] order_ack={account="A1", order_id=1001, type=CANCEL_ORDER, origin=GATEWAY, status=FORWARDED, error=UNDEFINED, text="", gateway_order_id=10000001, external_order_id="4237332692", request_id="roq-1595529571-17"}
I0723 20:40:47.052792 17135 strategy.cpp:225] order_ack={account="A1", order_id=1001, type=CANCEL_ORDER, origin=EXCHANGE, status=ACCEPTED, error=UNDEFINED, text="canceled", gateway_order_id=10000001, external_order_id="4237332692", request_id="roq-1595529571-17"}
I0723 20:40:47.052813 17135 strategy.cpp:233] order_update={account="A1", order_id=1001, exchange="deribit", symbol="BTC-PERPETUAL", status=CANCELED, side=BUY, price=9608.0, remaining_quantity=1.0, traded_quantity=0.0, position_effect=UNDEFINED, order_template="", create_time_utc=1595529586984000000ns, update_time_utc=1595529617014000000ns, gateway_order_id=10000001, external_order_id="4237332692"}
I0723 20:40:47.052819 17135 strategy.cpp:104] *** FINISHED ***
I0723 20:40:47.053414 17135 strategy.cpp:233] order_update={account="A1", order_id=1001, exchange="deribit", symbol="BTC-PERPETUAL", status=CANCELED, side=BUY, price=9608.0, remaining_quantity=1.0, traded_quantity=0.0, position_effect=UNDEFINED, order_template="", create_time_utc=1595529586984000000ns, update_time_utc=1595529647041000000ns, gateway_order_id=10000001, external_order_id="4237332692"}
W0723 20:40:47.053758 17136 controller.cpp:162] Signal 15 (Terminated)
I0723 20:40:47.053765 17136 controller.cpp:158] Event loop thread has terminated
I0723 20:40:47.053766 17135 controller.cpp:118] Waiting for event loop thread to terminate...
I0723 20:40:47.053810 17135 controller.cpp:121] Done!
I0723 20:40:47.054032 17135 application.cpp:47] ===== STOP =====
```

Some noteworthy observations

* The client will automatically receive download information immediately
  after an established connection to a market gateway. This allows the client
  to initialize internal state before any order is being placed.
* The client must use its own strictly increasing number scheme (per market
  gateway) to identify orders. (This number scheme is completely unrelated to
  any other connected client). The market gateway will use its own internal
  number scheme to manage all client orders. All order updates will include
  bother order identifiers allowing you to correlate logs. In this case the
  client uses 1001 to refer to the order. The gateway informs the client that
  the its internal identifier 10000001.
* Acknowledgement may be received up to 2 times. The first acknowledgement is
  the result of the market gateway validating order parameters and its own
  connection status to the exchange. The second acknowledgement originates from
  the exchange. This design allows the client to detect lost messages as early
  as possible. (Latency between client and gateway is significantly lower than
  latency between gateway and exchange).
* Logging has a cost -- avoid if you don't need it :-)


## License

The project is released under the terms of the BSD 3-Clause license.


## Links

* [Documentation](https://roq-trading.com/docs)
* [Contact us](mailto:info@roq-trading.com)
