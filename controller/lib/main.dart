import 'package:flutter/material.dart';
import 'dart:io';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Snake Game Controller',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: SnakeController(),
    );
  }
}

class SnakeController extends StatefulWidget {
  @override
  _SnakeControllerState createState() => _SnakeControllerState();
}

class _SnakeControllerState extends State<SnakeController> {
  late Socket socket;
  late TextEditingController ipController;

  @override
  void initState() {
    super.initState();
    ipController = TextEditingController();
    _showIpInputDialog();
  }

  void _showIpInputDialog() {
    WidgetsBinding.instance!.addPostFrameCallback((_) {
      showDialog(
        context: context,
        builder: (BuildContext context) {
          return AlertDialog(
            title: Text("Enter Server IP"),
            content: TextField(
              controller: ipController,
              keyboardType: TextInputType.numberWithOptions(decimal: true),
              decoration: InputDecoration(hintText: "Enter IP Address"),
            ),
            actions: <Widget>[
              TextButton(
                child: Text('Connect'),
                onPressed: () {
                  Navigator.of(context).pop();
                  _connectToServer(ipController.text);
                },
              ),
            ],
          );
        },
      );
    });
  }

  void _connectToServer(String ipAddress) async {
    try {
      socket = await Socket.connect(ipAddress, 65432);
      print('Connected to server');
      socket.listen((List<int> event) {
        print('Received: ${String.fromCharCodes(event)}');
      }, onError: (error) {
        print('Socket error: $error');
      }, onDone: () {
        print('Socket closed');
      });
    } catch (e) {
      print('Error: $e');
    }
  }

  void sendDirection(String direction) {
    if (socket != null) {
      socket.write(direction);
      print('Direction sent: $direction');
    } else {
      print('Socket not connected');
    }
  }

  void resetGame() {
    if (socket != null) {
      socket.write('R');
      print('Reset sent');
    } else {
      print('Socket not connected');
    }
  }

  @override
  void dispose() {
    socket.close();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Snake Game Controller'),
      ),
      body: Row(
        mainAxisAlignment: MainAxisAlignment.spaceBetween,
        children: <Widget>[
          // Left side - D-pad
          Expanded(
            flex: 1,
            child: DPad(onDirectionChanged: (direction) {
              sendDirection(direction);
            }),
          ),
          // Right side - Reset button
          Expanded(
            flex: 1,
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                SizedBox(height: 20),
                ElevatedButton(
                  onPressed: () {
                    resetGame();
                  },
                  style: ElevatedButton.styleFrom(
                    shape: CircleBorder(),
                    padding: EdgeInsets.all(24),
                  ),
                  child: Text('RESET'),
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }
}

class DPad extends StatelessWidget {
  final Function(String) onDirectionChanged;

  DPad({required this.onDirectionChanged});

  @override
  Widget build(BuildContext context) {
    return Column(
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        Row(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            _buildButton('U', Icons.arrow_drop_up),
          ],
        ),
        Row(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            _buildButton('L', Icons.arrow_left),
            SizedBox(width: 20),
            _buildButton('R', Icons.arrow_right),
          ],
        ),
        Row(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            _buildButton('D', Icons.arrow_drop_down),
          ],
        ),
      ],
    );
  }

  Widget _buildButton(String direction, IconData icon) {
    return IconButton(
      iconSize: 64,
      icon: Icon(icon),
      onPressed: () => onDirectionChanged(direction),
    );
  }
}
