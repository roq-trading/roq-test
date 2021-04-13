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

### Download and Install Miniforge

#### Linux / X86\_64

```bash
wget -N https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-Linux-x86_64.sh

bash Miniforge3-Linux-x86_64.sh -b -u -p ~/conda

~/conda/bin/conda install -y \
    gxx_linux-64
```

### Download and Install Miniforge

#### Linux / Aarch64

```bash
wget -N https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-Linux-aarch64.sh

bash Miniforge3-Linux-aarch64.sh -b -u -p ~/conda

~/conda/bin/conda install -y \
    gxx_linux-aarch64
```

#### macOS / X86\_64

```bash
wget -N https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-MacOSX-x86_64.sh

bash Miniforge3-MacOSX-x86_64.sh -b -u -p ~/conda

~/conda/bin/conda install -y \
   clang_osx-64
```

### Install Dependencies

```bash
source ~/conda/bin/activate

conda install -y \
    git \
    cmake

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
I0413 15:53:32.295634 19778343 service.cpp:50] ===== START =====
I0413 15:53:32.296539 19778343 service.cpp:34] The metrics service will *not* be started
I0413 15:53:32.306871 19778343 controller.cpp:74] session_id="e719400e-3a30-488a-8375-0f1ae373fab6"
I0413 15:53:32.306892 19778343 controller.cpp:78] Dispatching...
I0413 15:53:32.306934 19778343 controller.cpp:82] Starting event loop thread...
I0413 15:53:32.307798 19778400 controller.cpp:113] Event loop thread is now running
I0413 15:53:33.311486 19778400 session_manager.cpp:50] Connecting "unix:///Users/thraneh/dev/roq-dev/roq-deribit/src/roq/deribit/deribit-test.sock"
I0413 15:53:33.312990 19778400 session.cpp:32] Adding name="deribit" (user_id=1)
I0413 15:53:33.314770 19778343 pollster.cpp:305] Adding name="deribit" (user_id=1)
I0413 15:53:33.315412 19778343 strategy.cpp:104] Connected
I0413 15:53:33.316269 19778343 strategy.cpp:116] Downloading market data ...
I0413 15:53:33.316562 19778343 strategy.cpp:140] gateway_status={account="", supported=0x6f, available=0x7, unavailable=0x68}
I0413 15:53:33.322476 19778343 strategy.cpp:126] download_end={account="", max_order_id=0}
I0413 15:53:33.322594 19778343 strategy.cpp:128] Download market data has COMPLETED
I0413 15:53:33.322609 19778343 strategy.cpp:119] Downloading account data ...
I0413 15:53:33.322618 19778343 strategy.cpp:140] gateway_status={account="A1", supported=0x107f0000, available=0x107f0000, unavailable=0x0}
I0413 15:53:33.322645 19778343 strategy.cpp:170] Order management is READY
I0413 15:53:33.322660 19778343 strategy.cpp:126] download_end={account="A1", max_order_id=1000}
I0413 15:53:33.322663 19778343 strategy.cpp:131] Download account data has COMPLETED
I0413 15:53:51.661352 19778343 strategy.cpp:140] gateway_status={account="", supported=0x6f, available=0x6f, unavailable=0x0}
I0413 15:53:51.661391 19778343 strategy.cpp:152] Market data is READY
I0413 15:53:51.661397 19778343 strategy.cpp:240] *** INSTRUMENT READY ***
I0413 15:53:52.364610 19778343 strategy.cpp:232] *** READY TO TRADE ***
I0413 15:53:52.365087 19778343 strategy.cpp:50] create_order={account="A1", order_id=1001, exchange="deribit", symbol="BTC-PERPETUAL", side=BUY, quantity=1, order_type=LIMIT, price=62928, time_in_force=GTC, position_effect=UNDEFINED, execution_instruction=UNDEFINED, stop_price=nan, max_show_quantity=nan, order_template=""}
I0413 15:53:52.366614 19778343 strategy.cpp:204] order_ack={stream_id=0, account="A1", order_id=1001, type=CREATE_ORDER, origin=GATEWAY, status=FORWARDED, error=UNDEFINED, text="", gateway_order_id=10000001, external_account="", external_order_id="", request_id="roq-1618321996-20"}
I0413 15:53:52.387040 19778343 strategy.cpp:204] order_ack={stream_id=0, account="A1", order_id=1001, type=CREATE_ORDER, origin=EXCHANGE, status=ACCEPTED, error=UNDEFINED, text="success", gateway_order_id=10000001, external_account="", external_order_id="5649382948", request_id="roq-1618321996-20"}
I0413 15:53:52.387097 19778343 strategy.cpp:210] order_update={stream_id=0, account="A1", order_id=1001, exchange="deribit", symbol="BTC-PERPETUAL", status=WORKING, side=BUY, price=62928, remaining_quantity=1, traded_quantity=0, position_effect=UNDEFINED, order_template="", create_time_utc=1618322032413000000ns, update_time_utc=1618322032413000000ns, gateway_order_id=10000001, external_account="", external_order_id="5649382948"}
I0413 15:53:52.488825 19778343 strategy.cpp:219] trade_update={stream_id=1, account="A1", order_id=1001, exchange="deribit", symbol="BTC-PERPETUAL", side=BUY, position_effect=UNDEFINED, order_template="", create_time_utc=1618322032514000000ns, update_time_utc=1618322032514000000ns, gateway_order_id=10000001, external_account="", external_order_id="5649382948", fills=[{quantity=1, price=62928, trade_id=20000001, gateway_trade_id=20000001, external_trade_id="BTC-PERPETUAL#47662302"}]}
I0413 15:53:52.489157 19778343 strategy.cpp:210] order_update={stream_id=0, account="A1", order_id=1001, exchange="deribit", symbol="BTC-PERPETUAL", status=COMPLETED, side=BUY, price=62928, remaining_quantity=0, traded_quantity=1, position_effect=UNDEFINED, order_template="", create_time_utc=1618322032413000000ns, update_time_utc=1618322032514000000ns, gateway_order_id=10000001, external_account="", external_order_id="5649382948"}
I0413 15:53:52.489370 19778343 strategy.cpp:85] *** FINISHED ***
^CW0413 15:54:04.370765 19778400 controller.cpp:123] Signal 2 (Interrupt: 2)
I0413 15:54:04.370811 19778343 controller.cpp:88] Waiting for event loop thread to terminate...
I0413 15:54:04.370812 19778400 controller.cpp:119] Event loop thread has terminated
I0413 15:54:04.372147 19778343 controller.cpp:91] Done!
I0413 15:54:04.384713 19778343 service.cpp:54] ===== STOP =====
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

* [Roq GmbH](https://roq-trading.com/)
* [Documentation](https://roq-trading.com/docs/)
* [Contact us](mailto:info@roq-trading.com)
