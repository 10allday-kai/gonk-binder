/* (c) 2020 KAI OS TECHNOLOGIES (HONG KONG) LIMITED All rights reserved. This
 * file or any portion thereof may not be reproduced or used in any manner
 * whatsoever without the express written permission of KAI OS TECHNOLOGIES
 * (HONG KONG) LIMITED. KaiOS is the trademark of KAI OS TECHNOLOGIES (HONG
 * KONG) LIMITED or its affiliate company and may be registered in some
 * jurisdictions. All other trademarks are the property of their respective
 * owners.
 */

package b2g.connectivity;

import b2g.connectivity.NetworkInfoParcel;

/**
 * Connectivity unsolicited events.
 *
 */
oneway interface IConnectivityEventListener {

  /**
   * Notifies active network had been changed.
   */
  void onActiveNetworkChanged(in NetworkInfoParcel networkInfo);

  /**
   * Notifies network had been changed.
   */
  void onNetworkChanged(in NetworkInfoParcel networkInfo);
}
