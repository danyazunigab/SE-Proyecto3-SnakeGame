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

  @override
  void initState() {
    super.initState();
    connectToServer();
  }

  void connectToServer() async {
    try {
      socket = await Socket.connect('192.168.134.128', 65432); //CONFIGURAR IP PORQUE ESTA FUE DE PRUEBA XD
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
