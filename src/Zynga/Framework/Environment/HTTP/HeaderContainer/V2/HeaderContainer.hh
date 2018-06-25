<?hh // strict

namespace Zynga\Framework\Environment\HTTP\HeaderContainer\V2;

use Zynga\Framework\Environment\HTTP\HeaderContainer\V2\Interfaces\HeaderContainerInterface;
use Zynga\Framework\Environment\HTTP\HeaderContainer\V2\Exceptions\UnknownHeaderException;

class HeaderContainer implements HeaderContainerInterface {

  private Map<string, string> $_headers;

  public function __construct() {
    $this->_headers = Map {};
  }

  public function getHeader(string $header): string {
    if ($this->_headers->containsKey($header) === true) {
      return $this->_headers[$header];
    }
    throw new UnknownHeaderException('header='.$header);
  }

  public function setHeader(string $header, string $value): bool {
    $this->_headers[$header] = $value;
    return true;
  }

  public function noCaching(): bool {
    $this->setHeader('Cache-Control', 'no-store, no-cache, must-revalidate');
    $this->setHeader('Pragma', 'no-cache');
    $this->setHeader('Expires', 'Mon, 26 Jul 1997 05:00:00 GMT');
    return true;
  }

  public function contentIsJSON(): bool {
    $this->setHeader('Content-type', 'application/json');
    return true;
  }

  /**
   * PHP + hhvm do not provide a easy way to overload internal functions that
   * impact the outputstream.
   *
   * @codeCoverageIgnore
   */
  public function send(): bool {

    if (headers_sent() === true) {
      return false;
    }

    if ($this->_headers->count() > 0) {
      foreach ($this->_headers as $header => $value) {
        header($header.': '.$value);
      }
    }

    return true;

  }

}
