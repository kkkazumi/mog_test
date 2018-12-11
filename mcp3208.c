#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "mcp3208.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define MCP3208_START_BIT    (1 << 2)
#define MCP3208_SINGLE_ENDED (1 << 1)
#define MCP3208_DIFFERENTIAL (0 << 1)
#define MCP3208_CH_SHIFT     (6)

#define MCP3208_SPI_MODE        (SPI_CPOL | SPI_CPHA)
#define MCP3208_SPI_SPEED_HZ    1000000
#define MCP3208_SPI_DELAY_USECS 0
#define MCP3208_SPI_BITS        8

struct mcp3208 {
        int fd;
};

/**
 * 指定されたデバイスファイルをオープンする
 *
 * @param dev_path  MCP3208が接続されたspidevデバイスファイルへのパス。
 *
 * @return 成功するとstruct mcp3208へのポインタを返す。
 *         失敗するとNULLを返す。その際、適切なerrnoを設定する。
 */
struct mcp3208 *mcp3208_open(const char *dev_path)
{
        struct mcp3208 *adc;
        uint8_t mode = MCP3208_SPI_MODE;
        int error;
        int ret;

        if (dev_path == NULL) {
                errno = EINVAL;
                return NULL;
        }

        adc = calloc(1, sizeof(struct mcp3208));
        //adc = new sizeof(struct mcp3208);
        if (adc == NULL)
                return NULL;

        adc->fd = open(dev_path, O_RDWR);
        if (adc->fd < 0) {
                error = errno;
                goto err1;
        }

        /* 書き込み時のSPIモードを設定する */
        ret = ioctl(adc->fd, SPI_IOC_WR_MODE, &mode);
        if (ret < 0) {
                error = errno;
                goto err2;
        }

        /* 読み出し時のSPIモードを設定する */
        ret = ioctl(adc->fd, SPI_IOC_RD_MODE, &mode);
        if (ret < 0) {
                error = errno;
                goto err2;
        }

        return adc;

err2:
        close(adc->fd);
err1:
        free(adc);
        errno = errno;
        return NULL;
}

/**
 * 指定されたチャンネルのA/D変換結果を読み込む。
 * 読み込んだA/D変換結果をdigitに格納する。
 *
 * @param adc     オープン済みのMCP3208デバイスへのポインタ。
 * @param ch      サンプリングするチャンネル。
 * @param digit   A/D変換結果のデジタル値が格納される。
 *
 * @return 成功すると0を返し、voltageに電圧を格納する。
 *         失敗すると-1を返す。その際、適切な errno を設定する。
 */
int mcp3208_read(struct mcp3208 *adc, int ch, uint16_t *digit)
{
        uint8_t tx[3] = {0, };
        uint8_t rx[3] = {0, };
        struct spi_ioc_transfer tr;
        int ret;

        if (adc == NULL || digit == NULL ||
            ch < MCP3208_CH_MIN || MCP3208_CH_MAX < ch) {
                errno = EINVAL;
                return -1;
        }

        /* 送信バッファにスタートビット、SGL/DIFF*、D2、D1、D0をセットする */
        tx[0]            = MCP3208_START_BIT | MCP3208_SINGLE_ENDED;
        tx[1]            = ch << MCP3208_CH_SHIFT;

        /* 転送設定をセットする */
        tr.tx_buf        = (unsigned long)tx;
        tr.rx_buf        = (unsigned long)rx;
        tr.len           = ARRAY_SIZE(tx);
        tr.delay_usecs   = MCP3208_SPI_DELAY_USECS;
        tr.speed_hz      = MCP3208_SPI_SPEED_HZ;
        tr.bits_per_word = MCP3208_SPI_BITS;
        tr.cs_change     = 0;

        /* 全二重通信をおこなう */
        ret = ioctl(adc->fd, SPI_IOC_MESSAGE(1), &tr);
        if (ret < 1)
                return -1;

        /* 受信バッファからA/D変換結果を取り出す */
        *digit = (rx[1] & 0x0f) << 8;
        *digit |= rx[2];

        return 0;
}

/**
 * 指定されたMCP3208デバイスをクローズする
 *
 * @param adc オープン済みのMCP3208デバイスへのポインタ。
 *
 * @return 成功すると0を返す。
 *         失敗すると-1を返す。その際、適切なerrnoを設定する。
 */
int mcp3208_close(struct mcp3208 *adc)
{
        int fd;

        if (adc == NULL) {
                errno = EINVAL;
                return -1;
        }

        fd = adc->fd;
        free(adc);

        return close(fd);
}
