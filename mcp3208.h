#ifndef MCP3208_H
#define MCP3208_H

#include <stdint.h>

#define MCP3208_RESOLUTION_BITS 12 /* MCP3208の分解能(bit) */

#define MCP3208_CH_MIN 0 /* MCP3208で指定できるアナログ入力チャンネルの最小値 */
#define MCP3208_CH_MAX 3 /* MCP3208で指定できるアナログ入力チャンネルの最大値 */

struct mcp3208;

struct mcp3208 *mcp3208_open(const char *dev_path);
int mcp3208_read(struct mcp3208 *adc, int ch, uint16_t *digit);
int mcp3208_close(struct mcp3208 *adc);

#endif /* MCP3208_H */
