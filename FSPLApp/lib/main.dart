import 'package:flutter/material.dart';
import 'dart:math';

void main() {
  runApp(const FSPLApp());
}

class FSPLApp extends StatelessWidget {
  const FSPLApp({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'FSPL Calculator',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: const FSPLHomePage(),
    );
  }
}

class FSPLHomePage extends StatefulWidget {
  const FSPLHomePage({Key? key}) : super(key: key);

  @override
  State<FSPLHomePage> createState() => _FSPLHomePageState();
}

class _FSPLHomePageState extends State<FSPLHomePage> {
  final _distanceController = TextEditingController();
  final _frequencyController = TextEditingController();
  String? _result;
  String? _error;

  @override
  void dispose() {
    _distanceController.dispose();
    _frequencyController.dispose();
    super.dispose();
  }

  double _calculateFspl(double distanceMeters, double frequencyHz) {
    const c = 299792458.0;
    final ratio = 4 * pi * distanceMeters * frequencyHz / c;
    return 20 * log(ratio) / ln10;
  }

  void _onCalculate() {
    setState(() {
      _error = null;
      _result = null;
      final dText = _distanceController.text.trim();
      final fText = _frequencyController.text.trim();
      if (dText.isEmpty || fText.isEmpty) {
        _error = 'Please enter both distance and frequency.';
        return;
      }
      final d = double.tryParse(dText);
      final f = double.tryParse(fText);
      if (d == null || f == null || d <= 0 || f <= 0) {
        _error = 'Invalid numbers; ensure values > 0.';
        return;
      }
      final loss = _calculateFspl(d, f);
      _result = '${loss.toStringAsFixed(2)} dB';
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Free-Space Path Loss'),
      ),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            TextField(
              controller: _distanceController,
              keyboardType: TextInputType.numberWithOptions(decimal: true),
              decoration: const InputDecoration(
                labelText: 'Distance (meters)',
              ),
            ),
            const SizedBox(height: 12),
            TextField(
              controller: _frequencyController,
              keyboardType: TextInputType.numberWithOptions(decimal: true),
              decoration: const InputDecoration(
                labelText: 'Frequency (hertz)',
              ),
            ),
            const SizedBox(height: 24),
            ElevatedButton(
              onPressed: _onCalculate,
              child: const Text('Calculate'),
            ),
            const SizedBox(height: 24),
            if (_error != null) ...[
              Text(
                _error!,
                style: const TextStyle(color: Colors.red),
              ),
            ],
            if (_result != null) ...[
              Text(
                'FSPL: $_result',
                style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
              ),
            ],
          ],
        ),
      ),
    );
  }
}
