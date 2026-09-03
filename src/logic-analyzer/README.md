# logic-analyzer

Logic analyzer for a 65C02 CPU under test. Taps address/data/RWB/SYNC/IRQB/RESB lines through MCP23017 expanders, generates the CPU clock (manual pulse, automatic 1 Hz–100 Hz, or external), and streams bus states over a compact binary serial protocol (length-prefixed, CRC-8).

- Protocol: [docs/binary_protocol.md](../../docs/binary_protocol.md)
- Python client: `tools/logic_analyzer_client.py`
- Debug firmware (diagnostics on separate Serial1): `pio run -e logic-analyzer-debug`

```bash
pio run -e logic-analyzer -t upload
```
